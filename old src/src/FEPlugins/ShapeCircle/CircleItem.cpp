//
// Created by unbd_ on 2025/5/4.
//

#include "CircleItem.h"

#include "qgraphicssceneevent.h"
#include "qvector2d.h"
#include <qline.h>
#include <qpoint.h>
#include <qdebug.h>

CircleItem::CircleItem(const QRectF &rect, QGraphicsItem *parent)
    : CanvasElement(rect, parent) {
    pluginName = "ShapeCircle";
}

void CircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    CanvasElement::paint(painter, option, widget);
}

QPainterPath CircleItem::constructShape() const {
    QPainterPath path;
    QRectF rect(points[0], points[1]);
    path.addEllipse(rect);
    this->rect = rect;
    return path;
}

void CircleItem::updatePoints(QRectF &oldRect, QRectF &newRect) {
    CanvasElement::updatePoints(oldRect, newRect);
}
