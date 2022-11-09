#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

#include <QBrush>
#include <QDebug>

#include "qgraphics_arc_item.hpp"

#include "interpreter_semantic_error.hpp"

QtInterpreter::QtInterpreter(QObject * parent): QObject(parent){
}

void QtInterpreter::parseAndEvaluate(QString entry){
  std::string parse = entry.toStdString();
  std::istringstream stream(parse);
  bool okay = Interpreter::parse(stream);
  if (okay) {
    //evaluate
    Expression exp;
    try {
      exp = Interpreter::eval();

      //draw graphics
      for (int i = 0; i < graphics.size(); i++) {
        QGraphicsItem *graphic;
        if (graphics[i].type == PointType) {
          graphic = new QGraphicsEllipseItem(qreal(graphics[i].value.point_value.x), 
                                            qreal(graphics[i].value.point_value.y), 2, 2);
        } else if (graphics[i].type == LineType) {
          graphic = new QGraphicsLineItem(qreal(graphics[i].value.line_value.first.x), 
                                          qreal(graphics[i].value.line_value.first.y),
                                          qreal(graphics[i].value.line_value.second.x), 
                                          qreal(graphics[i].value.line_value.second.y));
        } else if (graphics[i].type == ArcType) {
          graphic = new QGraphicsArcItem(qreal(graphics[i].value.arc_value.center.x), 
                                        qreal(graphics[i].value.arc_value.center.y),
                                        qreal(graphics[i].value.arc_value.start.x), 
                                        qreal(graphics[i].value.arc_value.start.y),
                                        qreal(graphics[i].value.arc_value.span));
        } 
        emit drawGraphic(graphic);
      }
      //display result
      QString open{"("};
      QString result = QString::fromStdString(exp.head.value.sym_value);
      QString close{")"};
      QString message = open + result + close;
      emit info(message);
    } catch (InterpreterSemanticError& e) {
      QString error_message = QString::fromStdString(e.what());
      emit error(error_message);
    }


  } else {
    QString error_message = "Error: Cannot parse instruction";
    emit error(error_message);
  }
}
