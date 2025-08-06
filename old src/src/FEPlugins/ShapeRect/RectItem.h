//
// Created by unbd_ on 2025/5/5.
//

#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include "CanvasBase/CanvasElement.h"


class RectItem : public CanvasElement {
public:
    explicit RectItem(const QRectF &rect = QRectF(),
                       QGraphicsItem *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath constructShape() const override;
};


#endif //IMAGEITEM_H
