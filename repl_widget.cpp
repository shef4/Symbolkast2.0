#include "repl_widget.hpp"

#include<QDebug>
#include <QLabel>
#include <QLayout>
#include <QKeyEvent>

REPLWidget::REPLWidget(QWidget * parent): QWidget(parent){
  // make label
  auto repl_label = new QLabel("slisp>");
  // make line edit
  repl_txtfield = new QLineEdit();
  // make memory and point
  repl_memory = new ReplMemory();
  // layout the widgets
  auto repl_layout = new QHBoxLayout(this);
  repl_layout->addWidget(repl_label);
  repl_layout->addWidget(repl_txtfield);
  setLayout(repl_layout);
  // connect signals and slots
  connect(repl_txtfield, &QLineEdit::returnPressed, this, &REPLWidget::changed);
}

void REPLWidget::changed() {
  QString repl_input = repl_txtfield->text();
  //store latest line to memory
  repl_memory->new_entry(repl_input);
  emit lineEntered(repl_input);

  repl_txtfield->clear();
}

void REPLWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Up){
      // move back in history
      repl_txtfield->setText(repl_memory->prev_entry());
    }
    else if(event->key() == Qt::Key_Down){
      // move forward in history
      repl_txtfield->setText(repl_memory->next_entry());
    }
    else{
      // default handler for event
      QWidget::keyPressEvent(event);
    }
}

void REPLWidget::ReplMemory::new_entry(QString entry){
  if (!entry.isEmpty()) {
    memory->append(entry);
  }
  index = memory->size();
}

QString REPLWidget::ReplMemory::prev_entry(){
  QString entry = QString();
  if (index > 0){
    index--;
  }
  if (!memory->isEmpty()) {
    entry = memory->at(index);
  }
  return entry;
}

QString REPLWidget::ReplMemory::next_entry(){
  QString entry = QString();
  if (index < memory->size()-1){
    index++;
  }
  if (!memory->isEmpty()) {
    entry = memory->at(index);
  }
  return entry;
}
