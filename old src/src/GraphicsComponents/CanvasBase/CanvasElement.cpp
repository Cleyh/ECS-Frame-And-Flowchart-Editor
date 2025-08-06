//
// Created by unbd_ on 2025/5/6.
//
#include "CanvasElement.h"

#include "CommonFunctions.h"
#include "qstyle.h"
#include "qstyleoption.h"
#include "quuid.h"
#include "TypeDefine.h"
using TypeDefine::HOVER_COLOR;

CanvasElement::CanvasElement(const QRectF &rect, QGraphicsItem *parent): QGraphicsItem(parent), rect(rect) {
    setAcceptHoverEvents(true);
    properties->setProperty("fill_color", QColor(204, 150, 248, 128));
    properties->setProperty("border_color", QColor(204, 150, 248));
    properties->setProperty("border_width", 5);
    properties->setProperty("closed", true);
    properties->setProperty("text", "");
    properties->setProperty("text_color", QColor(0, 0, 0));
    properties->setProperty("text_size", 12);

    id_ = QUuid::createUuid().toString(QUuid::WithoutBraces);

    CanvasElement::addPoint(CanvasPointF(rect.topLeft()));
    CanvasElement::addPoint(CanvasPointF(rect.bottomRight()));
}

void CanvasElement::reserialize(const ReserializeData &data, QGraphicsItem *parent) {
    setParentItem(parent);
    this->pluginName = data.pluginName;
    this->id_ = data.id;
    this->type = data.type;
    this->properties = std::make_shared<PropertyType>(data.propertyType);
    this->points = data.points;
    this->obverseLinkPoints = data.obverseLinkPoints;
    this->selfLinkPoints = data.selfLinkPoints;
    this->rect = boundingRect();
}

void CanvasElement::setOnClickCallback(std::function<void()> callback) { onClickCallback = std::move(callback); }

void CanvasElement::setOnHoverCallback(
    std::function<void()> onEnter,
    std::function<void()> onMove,
    std::function<void()> onLeave) {
    onHoverEnterCallback = std::move(onEnter);
    onHoverMoveCallback = std::move(onMove);
    onHoverLeaveCallback = std::move(onLeave);
}

std::shared_ptr<PropertyType> CanvasElement::getProperties() const { return properties; }

QPointF CanvasElement::getNearestPoint(const QPointF &point) {
    QPointF localPos = mapFromScene(point);
    QList<QPointF> allPoints = CommonFunctions::discretizeWithBisection(constructShape());

    qreal minDist = INFINITY;
    QPointF closestPoint;

    for (const QPointF &p: allPoints) {
        auto len = QLineF(localPos, p).length();
        qreal dist = len * len; // 平方距离优化
        if (dist < minDist) {
            minDist = dist;
            closestPoint = p;
        }
    }
    return mapToScene(closestPoint);
}

TypeDefine::ToolType CanvasElement::getType() const {
    return type;
}

void CanvasElement::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    onClickCallback();
}

void CanvasElement::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
}

void CanvasElement::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
}

void CanvasElement::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    onHover = true;
    QGraphicsItem::hoverEnterEvent(event);
    onHoverEnterCallback();
}

void CanvasElement::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverMoveEvent(event);
    onHoverMoveCallback();
}

void CanvasElement::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    onHover = false;
    QGraphicsItem::hoverLeaveEvent(event);
    onHoverLeaveCallback();
}

void CanvasElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (isRectDirty) {
        updateBoundingRect();
        isRectDirty = false;
    }
    // 颜色参数
    auto path = constructShape();
    auto borderColor = properties->getProperty("border_color").value<QColor>();
    auto fillColor = properties->getProperty("fill_color").value<QColor>();
    auto borderWidth = properties->getProperty("border_width").toInt();
    auto isClosed = properties->getProperty("closed").toBool();

    // 文字相关
    auto text = properties->getProperty("text").toString();
    auto textColor = properties->getProperty("text_color").value<QColor>();
    auto textSize = properties->getProperty("text_size").toInt();

    QPen borderPen(borderColor, borderWidth);
    // 画笔设置
    borderPen.setStyle(Qt::SolidLine);
    painter->setPen(borderPen);
    if (isClosed) painter->setBrush(fillColor);
    painter->setRenderHint(QPainter::Antialiasing, true);
    // 绘制路径
    painter->drawPath(path);

    // 绘制文字
    if (!text.isEmpty()) {
        painter->setPen(textColor);
        painter->setFont(QFont("Arial", textSize));
        QRectF textRect = path.boundingRect();
        painter->drawText(textRect, Qt::AlignCenter, text);
    }

    // 绘制hover效果
    if (onHover && !isSelected()) {
        QPen hoverPen(HOVER_COLOR, 5);
        hoverPen.setStyle(Qt::SolidLine);
        painter->setPen(hoverPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path);
    }
}

QPainterPath CanvasElement::shape() const {
    return constructShape();
}

QRectF CanvasElement::boundingRect() const {
    // if (isBoundingRectDirty) {
    //     updateBoundingRect();
    //     isBoundingRectDirty = false;
    // }
    // return rect;
    return constructShape().boundingRect();
}

/**
     * 修改包围盒
     * 同时会更新点集
     */
void CanvasElement::setRect(const QRectF &rect) {
    prepareGeometryChange();
    QRectF oldRect = this->rect;
    this->rect = rect;
    updatePoints(oldRect, this->rect);
    updateSelfLinkPoints();
    updateObverseLinkPoints();
    setTransformOriginPoint(rect.center());
    update();
}

CanvasPointF CanvasElement::getPoint(int index) {
    if (points.isEmpty()) {
        qDebug() << "CanvasElement::getPoint: points is empty";
        return CanvasPointF();
    }
    if (index >= points.size()) {
        qDebug() << "CanvasElement::getPoint: out of range";
        return CanvasPointF();
    }
    if (index == -1) {
        return points.first();
    }
    return points[index];
}

/**
     * 基于包围盒更新点集坐标
     */
void CanvasElement::updatePoints(QRectF &oldRect, QRectF &newRect) {
    // 计算尺寸变化比例（避免除零）
    qreal scaleX = (oldRect.width() != 0) ? newRect.width() / oldRect.width() : 1.0;
    qreal scaleY = (oldRect.height() != 0) ? newRect.height() / oldRect.height() : 1.0;

    if (!points.isEmpty()) {
        // 遍历所有点进行坐标转换
        for (CanvasPointF &point: points) {
            // 计算点相对于旧矩形原点的坐标
            qreal relX = point.x() - oldRect.x();
            qreal relY = point.y() - oldRect.y();
            // 应用缩放和平移变换
            point.setX(newRect.x() + relX * scaleX);
            point.setY(newRect.y() + relY * scaleY);
        }
    }

    if (!obverseLinkPoints.isEmpty()) {
        // 更新链接点
        for (auto &linkPoint: getObverseLinkPoints()) {
            auto sharedPoint = linkPoint.toStrongRef();
            if (sharedPoint) {
                // 计算点相对于旧矩形原点的坐标
                qreal relX = sharedPoint->x() - oldRect.x();
                qreal relY = sharedPoint->y() - oldRect.y();
                sharedPoint->setX(newRect.x() + relX * scaleX);
                sharedPoint->setY(newRect.y() + relY * scaleY);
            } else {
                removeObverseLinkPoint(linkPoint);
            }
        }
    }

    if (!selfLinkPoints.isEmpty()) {
        // 更新管理点
        for (auto &linkPoint: getSelfLinkPoints()) {
            auto sharedPoint = linkPoint.toStrongRef();
            if (sharedPoint) {
                // 计算点相对于旧矩形原点的坐标
                qreal relX = sharedPoint->x() - oldRect.x();
                qreal relY = sharedPoint->y() - oldRect.y();
                sharedPoint->setX(newRect.x() + relX * scaleX);
                sharedPoint->setY(newRect.y() + relY * scaleY);
            } else {
                removeSelfLinkPoint(linkPoint);
            }
        }
    }
}

QList<QWeakPointer<CanvasPointF> > CanvasElement::getObverseLinkPoints() {
    return obverseLinkPoints;
}

QWeakPointer<CanvasPointF> CanvasElement::getObverseLinkPoint(int index) {
    if (obverseLinkPoints.size() == 0 || index >= obverseLinkPoints.size()) {
        qWarning() << "CanvasElement::getObverseLinkPoint: out of range";
        return QWeakPointer<CanvasPointF>(nullptr);
    }
    if (index == -1) index = obverseLinkPoints.size() - 1;
    return obverseLinkPoints.value(index);
}

void CanvasElement::setObverseLinkPoint(const CanvasPointF &point, int index) {
    const auto obLinkPoint = getObverseLinkPoint(index);
    if (obLinkPoint.isNull()) {
        qWarning() << "CanvasElement::setObverseLinkPoint: out of range";
        return;
    }
    if (const auto sharedPoint = obLinkPoint.toStrongRef()) {
        sharedPoint->setX(point.x());
        sharedPoint->setY(point.y());
    } else {
        qDebug() << "CanvasElement::setObverseLinkPoint: point is null";
    }
}

void CanvasElement::addObverseLinkPoint(const QWeakPointer<CanvasPointF> &point, int index) {
    if (index == -1) {
        obverseLinkPoints.append(point);
        return;
    }
    if (index >= obverseLinkPoints.size()) {
        qDebug() << "CanvasElement::addObverseLinkPoint: out of range";
        return;
    }
    obverseLinkPoints.insert(index, point);
}

void CanvasElement::removeObverseLinkPoint(const QWeakPointer<CanvasPointF> &point) {
    obverseLinkPoints.removeOne(point);
}

void CanvasElement::updateObverseLinkPoints() {
    for (auto &linkPoint: obverseLinkPoints) {
        auto sharedPoint = linkPoint.toStrongRef();
        if (!sharedPoint) {
            removeObverseLinkPoint(linkPoint);
        }
    }
}

void CanvasElement::acceptSelfLinkPoints(const QList<QSharedPointer<CanvasPointF> > &point) {
    selfLinkPoints.clear();
    selfLinkPoints.append(point);
}

QList<QWeakPointer<CanvasPointF> > CanvasElement::getSelfLinkPoints() {
    updateSelfLinkPoints();
    auto result = QList<QWeakPointer<CanvasPointF> >();
    for (const auto &point: selfLinkPoints) {
        result.append(point);
    }
    return result;
}

QWeakPointer<CanvasPointF> CanvasElement::getSelfLinkPoint(int index) {
    if (selfLinkPoints.length() == 0 || index >= selfLinkPoints.length()) {
        qWarning() << "CanvasElement::getSelfLinkPoint: out of range";
        return QWeakPointer<CanvasPointF>(nullptr);
    }
    if (index == -1) {
        index = selfLinkPoints.size() - 1;
    }
    return selfLinkPoints.value(index);
}

void CanvasElement::setSelfLinkPoint(const CanvasPointF &point, int index) {
    const auto selfLinkPoint = getSelfLinkPoint(index);
    if (selfLinkPoint.isNull()) {
        qWarning() << "CanvasElement::setSelfLinkPoint: out of range";
        return;
    }
    if (const auto sharedPoint = selfLinkPoint.toStrongRef()) {
        sharedPoint->setX(point.x());
        sharedPoint->setY(point.y());
    } else {
        qDebug() << "CanvasElement::setSelfLinkPoint: point is null";
    }
}

QWeakPointer<CanvasPointF> CanvasElement::addSelfLinkPoint(const CanvasPointF &point, int index) {
    auto linkPoint = QSharedPointer<CanvasPointF>::create(point);
    if (index == -1) {
        selfLinkPoints.append(linkPoint);
        return linkPoint;
    }
    if (index >= selfLinkPoints.size()) {
        qDebug() << "CanvasElement::addSelfLinkPoint: out of range";
        return QWeakPointer<CanvasPointF>(nullptr);
    }
    selfLinkPoints.insert(index, linkPoint);
    return linkPoint;
}

void CanvasElement::removeSelfLinkPoint(const QWeakPointer<CanvasPointF> &point) {
    selfLinkPoints.removeOne(point);
}

void CanvasElement::updateSelfLinkPoints() {
    for (auto &point: selfLinkPoints) {
        if (!point) {
            point.reset();
            removeSelfLinkPoint(point);
        }
    }
}

/**
     * 设置点坐标
     * @param point 坐标点
     * @param index 点索引，默认-1表示最后一个点
     */
void CanvasElement::setPoint(const CanvasPointF &point, int index) {
    if (points.isEmpty()) {
        qDebug() << "CanvasElement::setPoint: points is empty";
        return;
    }
    if (index >= points.size()) {
        qDebug() << "CanvasElement::setPoint: out of range";
        return;
    }
    if (index == -1) {
        index = points.size() - 1;
    }
    prepareGeometryChange();
    points[index] = point;
    isRectDirty = true;
    update();
}

/**
     * 插入点
     * @param point 坐标点
     * @param index 索引，默认-1表示在最后插入
     */
void CanvasElement::addPoint(const CanvasPointF &point, const int index) {
    if (index == -1) points.append(point);
    else points.insert(index, point);
    prepareGeometryChange();
    isRectDirty = true;
    update();
}

QList<CanvasPointF> CanvasElement::getPoints() {
    return points;
}

void CanvasElement::removePoint(const int index) {
    prepareGeometryChange();
    if (index == -1) {
        points.removeLast();
    } else if (index >= 0 && index < points.size()) {
        points.removeAt(index);
    }
    isRectDirty = true;
    update();
}

void CanvasElement::updateBoundingRect() {
    rect = constructShape().boundingRect();
}

void CanvasElement::setType(const TypeDefine::ToolType newType) {
    type = newType;
}
