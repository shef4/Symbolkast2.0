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
          QPointF point(graphics[i].value.point_value.x -2, graphics[i].value.point_value.y-2);
          graphic = new QGraphicsEllipseItem(point.x(), point.y(), 2, 2);

        } else if (graphics[i].type == LineType) {
          QPointF first(graphics[i].value.line_value.first.x -2, graphics[i].value.line_value.first.y-2);
          QPointF second(graphics[i].value.line_value.second.x -2, graphics[i].value.line_value.second.y-2);
          graphic = new QGraphicsLineItem(first.x(), first.y(),second.x(), second.y());

        } else if (graphics[i].type == ArcType) {
          QPointF center(graphics[i].value.arc_value.center.x -2, graphics[i].value.arc_value.center.y-2);
          QPointF start(graphics[i].value.arc_value.start.x -2, graphics[i].value.arc_value.start.y-2);
          graphic = new QGraphicsArcItem(center.x(), center.y(),start.x(), start.y(), qreal(graphics[i].value.arc_value.span));

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
