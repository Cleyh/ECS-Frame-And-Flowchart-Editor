//
// Created by unbd_ on 2025/5/8.
//

#include "CanvasPointF.h"
#include "QRandomGenerator"
#include "quuid.h"

CanvasPointF::CanvasPointF(const QPointF &pointf,
                           const PointType type,
                           const QString &id)
    : QPointF(pointf), id(id), type(type) {
}

QPointF CanvasPointF::point() const {
    return QPointF(x(), y());
}
