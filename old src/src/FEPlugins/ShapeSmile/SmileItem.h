#ifndef SMILEITEM_H
#define SMILEITEM_H

#include <QGraphicsItem>
#include <QPainter>

#include "CanvasBase/CanvasElement.h"

class SmileItem : public CanvasElement {
public:
    explicit SmileItem(const QRectF &rect = QRectF(),
                       QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    QPainterPath constructShape() const override;
    static SmileItem *reserialize(const BasicElement &element);

private:
    QColor m_faceColor = Qt::yellow;
    QColor m_eyeColor = Qt::black;
};


#endif //SMILEITEM_H
