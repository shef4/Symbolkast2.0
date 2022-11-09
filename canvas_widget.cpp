#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLayout>

CanvasWidget::CanvasWidget(QWidget * parent): QWidget(parent){
  // make a label
  scene = new QGraphicsScene(this);
  // layout the widgets
  auto canvas_layout = new QHBoxLayout(this);
  auto view = new QGraphicsView(scene,this);
  canvas_layout->addWidget(view);

  setLayout(canvas_layout);
}

void CanvasWidget::addGraphic(QGraphicsItem * item){
  scene->addItem(item);
}