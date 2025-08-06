//
// Created by unbd_ on 2025/5/3.
//

#include "AdjustmentHandlerItem.h"
#include <qdebug.h>

#include "QGraphicsScene"
#include "TypeDefine.h"

AdjustmentHandlerItem::AdjustmentHandlerItem(CanvasElement *target, QGraphicsItem *parent)
    : CanvasElement(QRectF(), parent), targetItem(target), originalRect(target->boundingRect()) {
    qDebug() << "AdjustmentItem::AdjustmentItem";
    CanvasElement::setType(TypeDefine::ToolType::SELECT);
    setZValue(std::numeric_limits<qreal>::max()); // 确保在最上层
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(false);
    if (!targetItem) {
        qDebug() << "AdjustmentHandlerItem::AdjustmentHandlerItem: targetItem is null";
        return;
    }
    if (targetItem->getType() == TypeDefine::ToolType::LINK || targetItem->getType() == TypeDefine::ToolType::PATH) {
    }
}

AdjustmentHandlerItem::~AdjustmentHandlerItem() {
    // targetItem->setAcceptedMouseButtons(Qt::AllButtons);

    targetItem = nullptr;
}

QRectF AdjustmentHandlerItem::boundingRect() const {
    return targetItem->boundingRect().adjusted(-ADJUST, -ADJUST, ADJUST, ADJUST);
}

void AdjustmentHandlerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (targetItem->getType() != TypeDefine::ToolType::LINK && targetItem->getType() != TypeDefine::ToolType::PATH) {
        // 绘制调整框
        const QRectF rect = boundingRect().adjusted(ADJUST, ADJUST, -ADJUST, -ADJUST);
        painter->setPen(QPen(TypeDefine::HOVER_COLOR, BORDER_WIDTH, Qt::DotLine));
        painter->drawRect(rect);

        painter->setPen(QPen(TypeDefine::HOVER_COLOR, BORDER_WIDTH, Qt::SolidLine));
        painter->setBrush(Qt::white);
        painter->drawEllipse(rect.topLeft(), TRIGGER, TRIGGER);
        painter->drawEllipse(rect.topRight(), TRIGGER, TRIGGER);
        painter->drawEllipse(rect.bottomLeft(), TRIGGER, TRIGGER);
        painter->drawEllipse(rect.bottomRight(), TRIGGER, TRIGGER);
    } else {
        painter->setPen(QPen(TypeDefine::HOVER_COLOR, BORDER_WIDTH, Qt::SolidLine));
        painter->setBrush(Qt::white);

        for (auto &point: targetItem->getPoints()) {
            painter->drawEllipse(point, TRIGGER, TRIGGER);
        }
    }
}

QPainterPath AdjustmentHandlerItem::constructShape() const {
    QPainterPath path;
    const QRectF rect = targetItem->boundingRect();
    path.addRect(rect.adjusted(-ADJUST, -ADJUST, ADJUST, ADJUST));
    return path;
}

void AdjustmentHandlerItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    const QPointF pos = event->pos();
    activeHandle = getHandlePos(pos);
    QGraphicsItem::mousePressEvent(event);
}

void AdjustmentHandlerItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    prepareGeometryChange();
    if (activeHandle == PathAdjust) {
        handleOnHoverItem(event);
        justifyPoint(event, adjustNodeIndex);
        // 调整路径
        update();
        return;
    } else {
        const QPointF delta = event->pos() - event->lastPos();
        const QRectF rect = justifyRect(delta, targetItem->boundingRect());
        targetItem->setRect(rect);
    }
    update();
}

void AdjustmentHandlerItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    CanvasElement::mouseReleaseEvent(event);
}

AdjustmentHandlerItem::HandlePos AdjustmentHandlerItem::getHandlePos(const QPointF &pos) {
    QRectF rect = targetItem->boundingRect();
    HandlePos result;
    // 判断是否在调整框内
    if (rect.contains(pos)) {
        result = Move;
    } else {
        result = None;
    }
    // 升级工具类型
    int trigger = 10;
    if (targetItem->getType() == TypeDefine::ToolType::LINK || targetItem->getType() == TypeDefine::ToolType::PATH) {
        adjustNodeIndex = -1; // 重置索引
        auto points = targetItem->getPoints();
        for (int i = 0; i < points.size(); ++i) {
            if (QLineF(pos, points.value(i)).length() >= trigger) continue;
            result = PathAdjust;
            adjustNodeIndex = i;
            break;
        }
    } else if (QLineF(pos, rect.topLeft()).length() < trigger)
        result = TopLeft;
    else if (QLineF(pos, rect.topRight()).length() < trigger)
        result = TopRight;
    else if (QLineF(pos, rect.bottomLeft()).length() < trigger)
        result = BottomLeft;
    else if (QLineF(pos, rect.bottomRight()).length() < trigger)
        result = BottomRight;
    return result;
}

void AdjustmentHandlerItem::handleOnHoverItem(QGraphicsSceneMouseEvent *event) {
    auto mPos = event->scenePos();
    QList<QGraphicsItem *> items = scene()->items(mPos);
    auto *thisItem = static_cast<CanvasElement *>(this);
    CanvasElement *tempHoverItem = nullptr;
    for (auto &item: items) {
        if (!item) continue;
        auto *canvasItem = static_cast<CanvasElement *>(item);
        if (!canvasItem || canvasItem == thisItem) continue;
        tempHoverItem = canvasItem;
    }
    if (!tempHoverItem) hoverItem = nullptr;
    else hoverItem = tempHoverItem;
}

QRectF AdjustmentHandlerItem::justifyRect(const QPointF &delta, const QRectF &rect) {
    QRectF finalRect = QRectF(rect);
    switch (activeHandle) {
        case TopLeft:
            finalRect.setTopLeft(rect.topLeft() + delta);
            break;
        case TopRight:
            finalRect.setTopRight(rect.topRight() + delta);
            break;
        case BottomLeft:
            finalRect.setBottomLeft(rect.bottomLeft() + delta);
            break;
        case BottomRight:
            finalRect.setBottomRight(rect.bottomRight() + delta);
            break;
        case Move:
            finalRect.translate(delta);
            break;
        default:
            break;
    }
    // 边界约束
    if (finalRect.width() < TypeDefine::ELEMENT_MIN_SIZE) finalRect.setWidth(TypeDefine::ELEMENT_MIN_SIZE);
    if (finalRect.height() < TypeDefine::ELEMENT_MIN_SIZE) finalRect.setHeight(TypeDefine::ELEMENT_MIN_SIZE);
    return finalRect;
}

void AdjustmentHandlerItem::justifyPoint(const QGraphicsSceneMouseEvent *event, const int index) {
    if (adjustNodeIndex == -1) return;


    const auto delta = event->pos() - event->lastPos();
    const auto point = targetItem->getPoint(adjustNodeIndex);
    const auto finalPoint = point + delta;
    targetItem->setPoint(CanvasPointF(finalPoint), adjustNodeIndex);

    if (targetItem->getType() != TypeDefine::ToolType::LINK) return;

    // 更新链接点
    auto linkPoints = targetItem->getSelfLinkPoint(adjustNodeIndex).toStrongRef();

    if (!linkPoints) return;

    if (hoverItem == nullptr) {
        // 处理移出情况
        targetItem->addSelfLinkPoint(CanvasPointF(finalPoint), adjustNodeIndex);
        targetItem->removeSelfLinkPoint(linkPoints);
        linkPoints.reset();
        return;
    }

    auto p = CanvasPointF(hoverItem->getNearestPoint(event->pos()));
    auto weakPtr = hoverItem->getSelfLinkPoint(adjustNodeIndex);
    if (!p.isNull()) {
        weakPtr = targetItem->addSelfLinkPoint(p, adjustNodeIndex);
        hoverItem->addObverseLinkPoint(weakPtr);
    } else {
        targetItem->addSelfLinkPoint(CanvasPointF(finalPoint), adjustNodeIndex);
    }
    targetItem->removeSelfLinkPoint(linkPoints);
    linkPoints.reset();
}
