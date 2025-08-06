//
// Created by unbd_ on 2025/5/6.
//

#include "BindPointManager.h"


BindPointManager::BindPointManager(QObject *parent)
    : QObject(parent) {
}

BindPointManager::~BindPointManager() {
}

QSharedPointer<CanvasPointF> BindPointManager::createLinkPoint(const QPointF &pos) {
    auto point = QSharedPointer<CanvasPointF>::create(pos);
    point->setType(PointType::CONTROL);
    bindingPoints.append(point);
    return point;
}

void BindPointManager::destroyLinkPoint(const QSharedPointer<CanvasPointF> &point) {
    bindingPoints.removeAll(point);
}
