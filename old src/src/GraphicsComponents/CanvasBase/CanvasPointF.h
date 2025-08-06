//
// Created by unbd_ on 2025/5/8.
//

#ifndef CANVASPOINTF_H
#define CANVASPOINTF_H
#include "QPointF"
#include "QString"
#include "quuid.h"
#include "TypeDefine.h"

using TypeDefine::PointType;

class CanvasPointF : public QPointF {
public:
    explicit CanvasPointF(const QPointF &pointf = QPointF(),
                          const PointType type = PointType::DEFAULT,
                          const QString &id = QUuid::createUuid().toString(QUuid::WithoutBraces));


    QPointF point() const;

public:
    QString getId() const { return id; }
    PointType getType() const { return type; }

    void setType(const PointType newType) { type = newType; }

private:
    QString id;
    PointType type = PointType::DEFAULT;
};


#endif //CANVASPOINTF_H
