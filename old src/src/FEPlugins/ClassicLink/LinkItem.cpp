//
// Created by unbd_ on 2025/5/5.
//

#include "LinkItem.h"
#include <cmath>
#include <qdebug.h>

LinkItem::LinkItem(const QRectF &rect, QGraphicsItem *parent)
    : CanvasElement(rect, parent) {
    pluginName = "ClassicLink";
    CanvasElement::setType(TypeDefine::ToolType::LINK);
    CanvasElement::getProperties()->setProperty("vertical", false);
    CanvasElement::getProperties()->setProperty("closed", false);
    CanvasElement::addSelfLinkPoint(CanvasPointF(rect.topLeft()));
    CanvasElement::addSelfLinkPoint(CanvasPointF(rect.bottomRight()));
}

void LinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    CanvasElement::paint(painter, option, widget);
    updatePointsFromSelfLinkPoints();
}

QPainterPath LinkItem::constructShape() const {
    if (points.length() < 2) {
        return QPainterPath();
    }
    QPainterPath path;
    auto vertical = getProperties()->getProperty("vertical").toBool();
    auto delta = vertical
                     ? points[1].y() + points[0].y()
                     : points[1].x() + points[0].x();

    QPointF point1 = selfLinkPoints.value(0)->point();
    QPointF point4 = selfLinkPoints.value(1)->point();
    QPointF point2;
    QPointF point3;
    if (vertical) {
        point2 = QPointF(point1.x(), delta / 2);
        point3 = QPointF(point4.x(), delta / 2);
    } else {
        point2 = QPointF(delta / 2, point1.y());
        point3 = QPointF(delta / 2, point4.y());
    }
    path.moveTo(point1);
    path.lineTo(point2);
    path.lineTo(point3);
    path.lineTo(point4);
    return path;
}

QRectF LinkItem::boundingRect() const {
    // 基于点集+链接点
    QRectF rect = constructShape().boundingRect();
    this->rect = rect;
    return rect;
}

void LinkItem::updatePointsFromSelfLinkPoints() {
    updateSelfLinkPoints();
    for (int i = 0; i < selfLinkPoints.size(); ++i) {
        auto sharedPtr = selfLinkPoints.value(i);
        CanvasElement::setPoint(*sharedPtr, i);
    }
}

void LinkItem::setPoint(const CanvasPointF &point, int index) {
    CanvasElement::setPoint(point, index);
    CanvasElement::setSelfLinkPoint(point, index);
}

void LinkItem::setSelfLinkPoint(const CanvasPointF &point, int index) {
    CanvasElement::setPoint(point, index);
    CanvasElement::setSelfLinkPoint(point, index);
}
