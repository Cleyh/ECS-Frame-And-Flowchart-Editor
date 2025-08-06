#pragma once

#ifndef CANVASITEM_H
#define CANVASITEM_H
#include <QGraphicsItem>
#include <QVariant>
#include <qpainter.h>
#include <QPainterPath>
#include <functional>
#include <qdebug.h>
#include <QSharedPointer>
#include <QWeakPointer>
#include "CanvasPointF.h"
#include "TypeDefine.h"
#include "Basic/BasicPoint.h"
#include "PropertyManager/PropertyManager.h"
using TypeDefine::PointType;

struct ReserializeData {
    QString pluginName;
    QString id;
    TypeDefine::ToolType type;
    PropertyType propertyType;
    QList<CanvasPointF> points;
    QList<QWeakPointer<CanvasPointF> > obverseLinkPoints;
    QList<QSharedPointer<CanvasPointF> > selfLinkPoints;
};

/**
 * Canvas图元基类
 * constructShape()：必须要实现的函数，用来构建图元形状
 * paint()函数默认实现，重写时建议先调用父类的paint()函数，以保证hover效果的显示。
 */
class CanvasElement : public QGraphicsItem {
public:
    explicit CanvasElement(const QRectF &rect = QRectF(),
                           QGraphicsItem *parent = nullptr);
    /**
     * 默认反序列化函数
     * @param data 序列化数据
     * @param parent 父item
     */
    void reserialize(const ReserializeData &data, QGraphicsItem *parent = nullptr);

public:
    virtual void setOnClickCallback(std::function<void()> callback = [=]() { ; });
    virtual void setOnHoverCallback(std::function<void()> onEnter = [=]() { ; },
                                    std::function<void()> onMove = [=]() { ; },
                                    std::function<void()> onLeave = [=]() { ; });
    virtual std::shared_ptr<PropertyType> getProperties() const;
    virtual QPointF getNearestPoint(const QPointF &point);
    virtual TypeDefine::ToolType getType() const;
    QString getId() const { return id_; }
    QString getPluginName() const { return pluginName; }

protected: /* 原生事件响应 */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

public: /* 绘制 */
    virtual QPainterPath constructShape() const = 0;
    /**
     * 更新图元的包围盒，会自动更新三个点集的坐标：
     * - points
     * - obverseLinkPoints
     * - selfLinkPoints
     * @param rect 包围盒
     */
    virtual void setRect(const QRectF &rect);

public: /* 绘制重写 */
    /**
     * 默认直接调用constructShape()函数绘制路径
     */
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    /**
     * 默认使用constructShape()函数返回形状
     */
    virtual QPainterPath shape() const override;
    /**
     * 默认使用constructShape()函数返回包围盒
     */
    virtual QRectF boundingRect() const override;

public: /* 点集操作 */
    virtual QList<CanvasPointF> getPoints();

    virtual CanvasPointF getPoint(int index = -1);
    virtual void setPoint(const CanvasPointF &point, int index = -1);
    virtual void addPoint(const CanvasPointF &point, const int index = -1);
    virtual void removePoint(const int index = -1);
    virtual void updatePoints(QRectF &oldRect, QRectF &newRect);

public: /* 观察链接点操作 */
    virtual QList<QWeakPointer<CanvasPointF> > getObverseLinkPoints();
    virtual QWeakPointer<CanvasPointF> getObverseLinkPoint(int index = -1);
    virtual void setObverseLinkPoint(const CanvasPointF &point, int index = -1);
    virtual void addObverseLinkPoint(const QWeakPointer<CanvasPointF> &point, int index = -1);
    virtual void removeObverseLinkPoint(const QWeakPointer<CanvasPointF> &point);
    virtual void updateObverseLinkPoints();

public: /* 管理链接点操作 */
    virtual void acceptSelfLinkPoints(const QList<QSharedPointer<CanvasPointF> > &point);
    virtual QList<QWeakPointer<CanvasPointF> > getSelfLinkPoints();
    virtual QWeakPointer<CanvasPointF> getSelfLinkPoint(int index = -1);
    virtual void setSelfLinkPoint(const CanvasPointF &point, int index = -1);
    virtual QWeakPointer<CanvasPointF> addSelfLinkPoint(const CanvasPointF &point, int index = -1);
    virtual void removeSelfLinkPoint(const QWeakPointer<CanvasPointF> &point);
    virtual void updateSelfLinkPoints();

protected:
    virtual void updateBoundingRect();
    virtual void setType(const TypeDefine::ToolType newType);

protected:
    QString pluginName = "CanvasElement";
    QString id_;
    TypeDefine::ToolType type = TypeDefine::ToolType::SHAPE;

    std::function<void()> onClickCallback = [=]() { ; };
    std::function<void()> onHoverEnterCallback = [=]() { ; };
    std::function<void()> onHoverMoveCallback = [=]() { ; };
    std::function<void()> onHoverLeaveCallback = [=]() { ; };

    mutable bool onHover = false;
    mutable bool onSelected = false;

    /**
     * 包围盒；
     * 一般固定形状通过包围盒即可计算；
     * 包围盒更新时默认会更新点集的坐标；
     */
    mutable QRectF rect;
    mutable bool isRectDirty = false;

    /**
     * 点集；
     * 一般自定义形状时需要基于点集绘制；
     * 点集更新时默认也会更新包围盒；
     * 数据序列化时需要使用点集；
     */
    QList<CanvasPointF> points;

    /**
     * 属性集合
     */
    std::shared_ptr<PropertyType> properties = std::make_shared<PropertyType>();

    /**
     * 观察点集合
     */
    QList<QWeakPointer<CanvasPointF> > obverseLinkPoints;

    /**
     * 可管理链接点集合
     */
    QList<QSharedPointer<CanvasPointF> > selfLinkPoints;
};


#endif //CANVASITEM_H
