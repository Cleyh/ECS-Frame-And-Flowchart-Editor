//
// Created by unbd_ on 2025/5/6.
//

#include "HighlightHandlerItem.h"

#include <qdebug.h>
#include "qgraphicssceneevent.h"
#include "TypeDefine.h"

HighlightHandlerItem::HighlightHandlerItem(CanvasElement *target, QGraphicsItem *parent)
    : CanvasElement(target->boundingRect(), parent), targetItem(target) {
    qDebug() << "HoverItem::HoverItem";

    // 自身设置
    setZValue(std::numeric_limits<qreal>::max()); // 确保在最上层
    // setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);

    // 目标项设置
    if (targetItem) targetItem->setSelected(true);
}

HighlightHandlerItem::~HighlightHandlerItem() {
    qDebug() << "HoverItem::~HoverItem";
    if (targetItem) {
        targetItem->setSelected(false);
        targetItem = nullptr;
    }
}

void HighlightHandlerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    auto path = constructShape();
    int borderWidth = 5;
    QPen hoverPen(TypeDefine::HOVER_COLOR, borderWidth);
    hoverPen.setStyle(Qt::SolidLine);
    painter->setPen(hoverPen);
    painter->drawPath(path);

    if (nearestPoint.isNull()) return;
    painter->setBrush(Qt::white);
    painter->drawEllipse(nearestPoint, 10, 10);
}

QPainterPath HighlightHandlerItem::constructShape() const {
    if (targetItem) return targetItem->constructShape();
    return QPainterPath();
}

QPointF HighlightHandlerItem::getNearestPoint(const QPointF &point) {
    return targetItem ? targetItem->getNearestPoint(point) : QPointF();
}

void HighlightHandlerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    CanvasElement::hoverEnterEvent(event);
}

void HighlightHandlerItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    CanvasElement::hoverMoveEvent(event);
    auto point = targetItem ? targetItem->getNearestPoint(event->pos()) : QPointF();
    if (QLineF(event->pos(), point).length() < 50) {
        nearestPoint = point;
    } else {
        nearestPoint = QPointF();
    }
    update();
}

void HighlightHandlerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    CanvasElement::hoverLeaveEvent(event);
}
