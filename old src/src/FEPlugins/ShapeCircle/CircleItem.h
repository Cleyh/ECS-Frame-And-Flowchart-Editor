//
// Created by unbd_ on 2025/5/4.
//

#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H
#include "CanvasBase/CanvasElement.h"


class CircleItem : public CanvasElement {
public:
    explicit CircleItem(const QRectF &rect = QRectF(),
                        QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath constructShape() const override;
    void updatePoints(QRectF &oldRect, QRectF &newRect);

private:
    QPointF m_nearestPoint; // 最近点
};


#endif //CIRCLEITEM_H
