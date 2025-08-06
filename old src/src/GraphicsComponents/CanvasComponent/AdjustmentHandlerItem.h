#ifndef ADJUSTMENTITEM_H
#define ADJUSTMENTITEM_H
#include "../CanvasBase/CanvasElement.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <memory>
#include <QGraphicsRectItem>


class AdjustmentHandlerItem : public CanvasElement {
    enum HandlePos {
        TopLeft, TopRight, BottomLeft, BottomRight,
        Move,
        PathAdjust,
        None
    };

public:
    explicit AdjustmentHandlerItem(CanvasElement *target, QGraphicsItem *parent = nullptr);
    ~AdjustmentHandlerItem() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath constructShape() const override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    HandlePos getHandlePos(const QPointF &pos);
    void handleOnHoverItem(QGraphicsSceneMouseEvent* event);

    QRectF justifyRect(const QPointF &delta, const QRectF &rect);
    void justifyPoint(const QGraphicsSceneMouseEvent *event, int index);
private:
    int BORDER_WIDTH = 5; // 边框宽度
    int TRIGGER = 10; // 控制点触发范围
    int ADJUST = 20; // 调整框范围

    // CanvasPointF* adjustNode;
    int adjustNodeIndex = -1;
    HandlePos activeHandle = None;
    CanvasElement *targetItem;
    CanvasElement *hoverItem;
    QRectF originalRect;
};


#endif //ADJUSTMENTITEM_H
