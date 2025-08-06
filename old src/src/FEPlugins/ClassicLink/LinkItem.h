#ifndef LINKITEM_H
#define LINKITEM_H
#include "CanvasBase/CanvasElement.h"


class LinkItem : public CanvasElement {
public:
    explicit LinkItem(const QRectF &rect = QRectF(),
                      QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath constructShape() const override;
    QRectF boundingRect() const override;

private:
    void updatePointsFromSelfLinkPoints();

public:
    void setPoint(const CanvasPointF &point, int index = -1) override;
    void setSelfLinkPoint(const CanvasPointF &point, int index) override;
};


#endif //LINKITEM_H
