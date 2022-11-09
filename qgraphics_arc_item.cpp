#include "qgraphics_arc_item.hpp"

#include <cmath>

#include <QDebug>
#include <QPainter>

QGraphicsArcItem::QGraphicsArcItem(qreal center_x, qreal center_y, qreal start_x, qreal start_y, double span,
                                   QGraphicsItem *parent) {
  qreal radius = sqrt(pow((center_x - start_x), 2) + pow((center_y - start_y), 2));
  qreal boundingBoxX = center_x - radius;
  qreal boundingBoxY = center_y - radius;
  qreal box_size = radius * 2;
  double pi = atan2(0, -1);
  double startAngle = (16 * (atan2(center_y - start_y, start_x - center_x) * (180 / pi)));
  double spanAngle = (16 * (span * (180 / pi)));
  setRect(boundingBoxX, boundingBoxY, box_size, box_size);
  setStartAngle((int)startAngle);
  setSpanAngle((int)spanAngle);
}
void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->drawArc(rect(), startAngle(), spanAngle());
}