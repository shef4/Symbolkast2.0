#include "main_window.hpp"

#include <iostream>
#include <QLayout>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

MainWindow::MainWindow(QWidget * parent): MainWindow("", parent){
  //widget classes
  auto message_widget = new MessageWidget(this);
  auto canvas_widget = new CanvasWidget(this);
  auto repl_widget = new REPLWidget(this);
  //set layout
  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(message_widget);
  main_layout->addWidget(canvas_widget);
  main_layout->addWidget(repl_widget);
  setLayout(main_layout);
  setMinimumSize(800, 600);
  //connect signals and slots
  connect(repl_widget, &REPLWidget::lineEntered, &interp, &QtInterpreter::parseAndEvaluate);
  connect(&interp, &QtInterpreter::info, message_widget, &MessageWidget::info);
  connect(&interp, &QtInterpreter::error, message_widget, &MessageWidget::error);
  connect(&interp, &QtInterpreter::drawGraphic, canvas_widget, &CanvasWidget::addGraphic);
}

MainWindow::MainWindow(std::string filename, QWidget * parent): QWidget(parent){
  //widget classes
  auto message_widget = new MessageWidget(this);
  auto canvas_widget = new CanvasWidget(this);
  auto repl_widget = new REPLWidget(this);
  //set layout
  auto main_layout = new QVBoxLayout();
  main_layout->addWidget(message_widget);
  main_layout->addWidget(canvas_widget);
  main_layout->addWidget(repl_widget);
  setLayout(main_layout);
  setMinimumSize(800, 600);
  //connect signals and slots
  connect(repl_widget, &REPLWidget::lineEntered, &interp, &QtInterpreter::parseAndEvaluate);
  connect(&interp, &QtInterpreter::info, message_widget, &MessageWidget::info);
  connect(&interp, &QtInterpreter::error, message_widget, &MessageWidget::error);
  connect(&interp, &QtInterpreter::drawGraphic, canvas_widget, &CanvasWidget::addGraphic);
  //run file
  if(!filename.empty()) {
    QString file_path = QString::fromStdString(filename);
    QFile file(file_path);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text)){
      QTextStream in(&file);
      QString filecontent = in.readAll();
      interp.parseAndEvaluate(filecontent);
    } else {
      QString error_message = "Error: opening file";
      emit interp.error(error_message);
    }
    file.close();
  }
  
}