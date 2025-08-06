#pragma once

#ifndef CANVASSPACE_H
#define CANVASSPACE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <qset.h>
#include <qstring.h>
#include <functional>
#include "CanvasBase/CanvasElement.h"
#include <qdebug.h>
#include <memory>

#include "Manager/BindPointManager.h"
#include "PropertyManager/PropertyManager.h"
#include "TypeDefine.h"
using TypeDefine::ToolType;


class CanvasSpace;

class ItemDriver {
public:
    ItemDriver(std::function<CanvasElement*(const QRectF &, QGraphicsItem *)> func)
        : createElementFunc(std::move(func)) { ; }

    CanvasElement *newElement(const QRectF &rect, QGraphicsItem *parent = nullptr) const {
        if (createElementFunc) return createElementFunc(rect, parent);
        qWarning() << "ItemDriver::newElement: createElement is null";
        return nullptr;
    }

private:
    std::function<CanvasElement*(const QRectF &, QGraphicsItem *)> createElementFunc;
};

class Tool {
public:
    const QString name;
    const ToolType type;
    const ItemDriver *itemDriver = nullptr;

    explicit Tool(const QString &name,
                  const ToolType type = ToolType::SELECT,
                  const ItemDriver *item_driver = nullptr)
        : name(name),
          type(type),
          itemDriver(item_driver) {
    }

    ~Tool() {
        if (itemDriver) delete itemDriver;
        itemDriver = nullptr;
    };
};

class CanvasSpace : public QGraphicsView {
    Q_OBJECT

public:
    explicit CanvasSpace(QWidget *parent = nullptr);
    void loadFromData(const QVector<BasicElement> &elements);
    void resetCanvas();
    QImage getImage();
    void addItem(QGraphicsItem *pItem) const;
    void addGuideLine(qreal pos, bool isVertical);
    void clearGuideLines();
    QList<CanvasElement *> getCanvasElements();
    ~CanvasSpace() override = default;

signals:
    void itemSelected(CanvasElement *item);

public: /* 属性相关 */
    std::shared_ptr<PropertyType> getCurrentProperties() const;
    std::shared_ptr<PropertyType> getCanvasProperties() const;

public: /* 工具相关 */
    void addTool(Tool *const tool);
    bool selectTool(Tool *const tool);
    Tool *getSelectedTool();
    CanvasElement *getSelectedElement() const;

public: /* 图元操作相关 */
    CanvasElement *createItem(const ItemDriver *itemDriver, const QRectF &rect, QGraphicsItem *parent);
    void selectItem(CanvasElement *item);
    void highlightItem(CanvasElement *item);
    void clearAdjustmentHandler();
    void clearHighlightHandler();

private: /* 事件响应 */
    void onSelectMousePress(QMouseEvent *event);
    void onSelectMouseMove(QMouseEvent *event);
    void onSelectKeyPress(QKeyEvent *event);

    void onShapeMousePress(QMouseEvent *event);
    void onShapeMouseMove(QMouseEvent *event);
    void onShapeMouseRelease(QMouseEvent *event);
    void onPathMousePress(QMouseEvent *event);
    void onPathMouseMove(QMouseEvent *event);
    void onPathMouseRelease(QMouseEvent *event);
    void onLinkMouseMove(QMouseEvent *event);
    void onLinkMousePress(QMouseEvent *event);
    void onLinkMouseRelease(QMouseEvent *event);

protected: /* 原生事件响应 */
    // void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

public:

private:
    Tool *selectedTool;
    QList<Tool *> tools;
    BindPointManager *bindPointManager = nullptr; // 绑定点管理器
    void applyZoom(qreal zoomFactor, QPointF mousePos);

private:
    QGraphicsScene *mainScene; // 主场景

    QGraphicsItemGroup *m_mainLayer; // 主图元层
    QGraphicsItemGroup *m_guideLayer; // 辅助线层
    QList<QGraphicsItem *> m_currentGuides; // 当前辅助线

    bool isZooming = false;
    qreal m_currentZoom; // 当前缩放系数
    const qreal MIN_ZOOM = 1; // 最小缩放阈值
    const qreal MAX_ZOOM = 1.5; // 最大缩放阈值


    CanvasElement *tempItem = nullptr;
    bool m_isDrawing = false; // 标记是否正在绘制

    CanvasElement *adjustmentHandler = nullptr; // 当前调整框
    CanvasElement *selectedItem = nullptr; // 当前选中图元
    CanvasElement *highlightHandler = nullptr;
    CanvasElement *highlightedItem = nullptr;

    std::shared_ptr<PropertyType> properties = std::make_shared<PropertyType>(); // 属性集合
};


#endif //CANVASSPACE_H
