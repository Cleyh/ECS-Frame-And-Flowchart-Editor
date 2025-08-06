#ifndef HOVERITEM_H
#define HOVERITEM_H
#include "CanvasBase/CanvasElement.h"


class HighlightHandlerItem : public CanvasElement {
public:
    explicit HighlightHandlerItem(CanvasElement *target = nullptr, QGraphicsItem *parent = nullptr);
    ~HighlightHandlerItem() override;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath constructShape() const override;
    QPointF getNearestPoint(const QPointF &point) override;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    CanvasElement *targetItem;
    QPointF nearestPoint; // 最近点
};


#endif //HOVERITEM_H
