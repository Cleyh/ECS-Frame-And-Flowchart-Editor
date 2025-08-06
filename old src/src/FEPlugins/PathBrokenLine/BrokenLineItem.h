//
// Created by unbd_ on 2025/5/4.
//

#ifndef BROKENLINEITEM_H
#define BROKENLINEITEM_H
#include "CanvasBase/CanvasElement.h"


class BrokenLineItem : public CanvasElement {
public:
    explicit BrokenLineItem(const QRectF &rect = QRectF(), QGraphicsItem *parent = nullptr);

    QPainterPath constructShape() const override;
};


#endif //BROKENLINEITEM_H
