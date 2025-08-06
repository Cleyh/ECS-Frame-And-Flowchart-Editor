#include "CanvasSpace.h"
#include "CanvasConvertor.h"
#include "QApplication"
#include "CanvasComponent/AdjustmentHandlerItem.h"
#include "CanvasComponent/HighlightHandlerItem.h"
#include <QScrollBar>

CanvasSpace::CanvasSpace(QWidget *parent) : QGraphicsView(parent) {
    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(0, 0, 20000, 20000);
    mainScene->setBackgroundBrush(QColor(245, 245, 245));
    setScene(mainScene);
    // m_guideLayer = new QGraphicsItemGroup; // 辅助线层
    // m_guideLayer->setZValue(INT64_MAX); // 确保辅助线层在主图元层之上
    // m_guideLayer->setHandlesChildEvents(false); // 禁用子项事件处理

    setDragMode(NoDrag);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse); // 缩放时以鼠标位置为中心
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(mainScene, &QGraphicsScene::sceneRectChanged,
            [this](const QRectF &rect) {
                // 保持至少保留20px的边界空间
                setSceneRect(rect.adjusted(-20, -20, 20, 20));
            });

    setInteractive(true); // 启用交互功能
    setMouseTracking(true);
    bindPointManager = new BindPointManager(this);

    // m_scene->addItem(m_mainLayer);
    // m_mainLayer->setHandlesChildEvents(true);
    // m_mainLayer->setFlag(QGraphicsItem::ItemIsSelectable, false);
    // m_scene->addItem(m_guideLayer);

    // 可选：优化渲染提示
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(FullViewportUpdate); // 按需调整
}

void CanvasSpace::loadFromData(const QVector<BasicElement> &elements) {
    selectItem(nullptr);
    highlightItem(nullptr);
    auto canvasConvertor = CanvasConvertor{tools};
    auto itemList = canvasConvertor.convertToCanvasElements(elements);
    mainScene->clear();
    for (auto item: itemList) {
        mainScene->addItem(item);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

        item->setOnClickCallback([=]() {
            selectItem(item);
        });
        item->setOnHoverCallback(
            [=]() {
                highlightItem(item);
            },
            [=]() {
            },
            [=]() {
                // highlightItem(nullptr);
            }
        );
    }
}

void CanvasSpace::resetCanvas() {
    selectItem(nullptr);
    highlightItem(nullptr);
    mainScene->clear();
}

QImage CanvasSpace::getImage() {
    selectItem(nullptr);
    highlightItem(nullptr);

    QGraphicsScene *scene = this->scene();
    if (!scene || scene->items().isEmpty()) {
        qWarning() << "Scene is empty or invalid";
        return {};
    }

    // 计算所有可见item的包围矩形
    QRectF totalRect;
    bool first = true;

    foreach(QGraphicsItem* item, scene->items()) {
        if (!item->isVisible()) continue; // 如果需要包含隐藏项则移除这个判断

        QRectF itemRect = item->sceneBoundingRect();
        if (first) {
            totalRect = itemRect;
            first = false;
        } else {
            totalRect = totalRect.united(itemRect);
        }
    }

    if (first) {
        // 没有找到可见项
        qWarning() << "No visible items found";
        return {};
    }

    // 创建目标图像（透明背景）
    QImage image(totalRect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    // 使用QPainter渲染场景到图像
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter,
                  QRectF(), // 目标区域为整个图像
                  totalRect, // 源区域为计算的包围矩形
                  Qt::KeepAspectRatioByExpanding);
    painter.end();

    return image;
}

void CanvasSpace::addItem(QGraphicsItem *pItem) const {
    mainScene->addItem(pItem); // 将图元添加到场景
    // m_mainLayer->addToGroup(pItem); // 将图元添加到主图元层
}

void CanvasSpace::addGuideLine(qreal pos, bool isVertical) {
    QGraphicsLineItem *line = new QGraphicsLineItem;
    line->setPen(QPen(Qt::blue, 1, Qt::DashLine));

    if (isVertical) {
        line->setLine(pos, -200, pos, 200); // 垂直无限长线
    } else {
        line->setLine(-200, pos, 200, pos); // 水平无限长线
    }

    line->setParentItem(m_guideLayer);
    m_currentGuides.append(line);
}

void CanvasSpace::clearGuideLines() {
    for (QGraphicsItem *guide: m_currentGuides) {
        m_guideLayer->removeFromGroup(guide);
        delete guide; // 删除辅助线
    }
    m_currentGuides.clear();
}

QList<CanvasElement *> CanvasSpace::getCanvasElements() {
    auto rawItems = scene()->items();
    QList<CanvasElement *> items;
    for (auto item: rawItems) {
        if (!item) continue;
        const auto canvasItem = dynamic_cast<CanvasElement *>(item);
        if (canvasItem) {
            items.append(static_cast<CanvasElement *>(canvasItem));
        }
    }
    return items;
}

std::shared_ptr<PropertyType> CanvasSpace::getCurrentProperties() const {
    return selectedItem
               ? selectedItem->getProperties()
               : properties;
}

std::shared_ptr<PropertyType> CanvasSpace::getCanvasProperties() const {
    return properties;
}

void CanvasSpace::addTool(Tool *const tool) {
    qDebug() << "addTool: " << tool->name;
    tools.append(tool);
}

bool CanvasSpace::selectTool(Tool *const tool) {
    if (!tools.contains(tool)) return false;
    qDebug() << "canvas: selectTool: " << tool->name;
    highlightItem(nullptr);
    selectItem(nullptr);
    switch (tool->type) {
        case ToolType::SELECT:
            break;
        case ToolType::SHAPE:
            break;
        case ToolType::PATH:
            break;
        case ToolType::LINK:
            break;
        default:
            break;
    }
    selectedTool = tool;
    return true;
}

Tool *CanvasSpace::getSelectedTool() {
    if (selectedTool == nullptr) {
        auto tool = tools[0];
        if (tool == nullptr) {
            tool = new Tool("cursor",
                            ToolType::SELECT);
        }
        selectedTool = tool;
    }
    return selectedTool;
}

CanvasElement *CanvasSpace::getSelectedElement() const {
    return selectedItem;
}

CanvasElement *CanvasSpace::createItem(const ItemDriver *itemDriver,
                                       const QRectF &rect,
                                       QGraphicsItem *parent) {
    CanvasElement *item = itemDriver->newElement(rect, parent);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    item->setOnClickCallback([=]() {
        selectItem(item);
    });

    item->setOnHoverCallback(
        [=]() {
            highlightItem(item);
        },
        [=]() {
        },
        [=]() {
            // highlightItem(nullptr);
        }
    );

    return item;
}

void CanvasSpace::selectItem(CanvasElement *item) {
    // 如果当前选中项是已选中项，则不处理
    if (item == selectedItem) return;

    // 切换选择
    if (selectedItem) selectedItem->setSelected(false);
    clearAdjustmentHandler();
    selectedItem = item;
    emit itemSelected(item);

    // 空选择
    if (selectedItem == nullptr || getSelectedTool()->type != ToolType::SELECT) return;

    // 选中项处理
    selectedItem->setSelected(true);
    adjustmentHandler = new AdjustmentHandlerItem(item, item);
    mainScene->addItem(adjustmentHandler);
}

void CanvasSpace::highlightItem(CanvasElement *item) {
    if (item == highlightedItem) return;

    if (highlightedItem) highlightedItem->setSelected(false);
    clearHighlightHandler();
    highlightedItem = item;

    if (!highlightedItem) return;

    if (getSelectedTool()->type != ToolType::SELECT) highlightedItem->setSelected(true);

    // LINK工具时可以高亮，SEL工具时只有持有的是LINK图元才高亮
    if (
        getSelectedTool()->type == ToolType::LINK
        // ||
        // (getSelectedTool()->type == ToolType::SELECT && selectedItem && selectedItem->getType() == ToolType::LINK)
    ) {
        highlightHandler = new HighlightHandlerItem(highlightedItem);
        scene()->addItem(highlightHandler);
    }
}

void CanvasSpace::clearAdjustmentHandler() {
    if (adjustmentHandler) {
        scene()->removeItem(adjustmentHandler);
        delete adjustmentHandler;
        adjustmentHandler = nullptr;
    }
}

void CanvasSpace::clearHighlightHandler() {
    if (highlightHandler) {
        scene()->removeItem(highlightHandler);
        delete highlightHandler;
        highlightHandler = nullptr;
    }
}

void CanvasSpace::onSelectMousePress(QMouseEvent *event) {
    m_isDrawing = false;
    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *clickedItem = scene()->itemAt(scenePos, QTransform());

    // 如果点击的是空白区域
    if (!clickedItem) {
        scene()->clearSelection(); // 清除所有图元选中状态
        selectItem(nullptr);
    }
}

void CanvasSpace::onSelectMouseMove(QMouseEvent *event) {
}

void CanvasSpace::onSelectKeyPress(QKeyEvent *event) {
    // 删除选中项
    if (event->key() == Qt::Key_Delete && selectedItem) {
        auto deletingItem = selectedItem;
        highlightItem(nullptr);
        selectItem(nullptr);
        scene()->removeItem(deletingItem);
        delete deletingItem;
        deletingItem = nullptr;
    } else if (event->key() == Qt::Key_PageUp && selectedItem) {
        getSelectedElement()->setZValue(getSelectedElement()->zValue() + 1);
        event->accept();
    } else if (event->key() == Qt::Key_PageDown && selectedItem) {
        getSelectedElement()->setZValue(getSelectedElement()->zValue() - 1);
        event->accept();
    }
}


void CanvasSpace::onShapeMousePress(QMouseEvent *event) {
    if (event->button() != Qt::LeftButton) return;
    QGraphicsView::mousePressEvent(event);
    event->accept();

    m_isDrawing = true;
    auto dragStartPos = mapToScene(event->pos());
    dragStartPos.setX(dragStartPos.x() - TypeDefine::ELEMENT_MIN_SIZE / 2);
    dragStartPos.setY(dragStartPos.y() - TypeDefine::ELEMENT_MIN_SIZE / 2);

    // 创建临时预览图元
    tempItem = getSelectedTool()->itemDriver->newElement(
        QRectF(dragStartPos, QSizeF(TypeDefine::ELEMENT_MIN_SIZE, TypeDefine::ELEMENT_MIN_SIZE)));
    tempItem->setZValue(1000); // 置于顶层
    tempItem->setOpacity(0.5); // 半透明预览
    scene()->addItem(tempItem);
}

void CanvasSpace::onShapeMouseMove(QMouseEvent *event) {
    if (m_isDrawing) {
        // 计算当前矩形范围
        QPointF currentPos = mapToScene(event->pos());
        QRectF faceRect(tempItem->boundingRect().topLeft(), currentPos);
        // 限制矩形范围
        faceRect = faceRect.normalized();
        // 边界约束
        if (faceRect.width() < TypeDefine::ELEMENT_MIN_SIZE) faceRect.setWidth(TypeDefine::ELEMENT_MIN_SIZE);
        if (faceRect.height() < TypeDefine::ELEMENT_MIN_SIZE) faceRect.setHeight(TypeDefine::ELEMENT_MIN_SIZE);
        // 更新临时图形尺寸
        tempItem->setRect(faceRect.normalized());
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CanvasSpace::onShapeMouseRelease(QMouseEvent *event) {
    if (m_isDrawing && tempItem) {
        // 创建最终图元
        CanvasElement *finalSmile = createItem(getSelectedTool()->itemDriver,
                                               tempItem->boundingRect(),
                                               tempItem->parentItem());
        finalSmile->setPos(tempItem->pos());

        // 添加到主图层
        addItem(finalSmile);

        // 清理临时图元
        scene()->removeItem(tempItem);
        delete tempItem;
        tempItem = nullptr;
        m_isDrawing = false;
    }
}

void CanvasSpace::onPathMousePress(QMouseEvent *event) {
    // 新增画线
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
        if (!tempItem) {
            // 创建临时预览图元
            const QPointF currentPos = mapToScene(event->pos());
            const QRectF rect(currentPos, currentPos);
            tempItem = getSelectedTool()->itemDriver->newElement(rect);
            tempItem->setZValue(1000); // 置于顶层
            tempItem->setOpacity(0.5); // 半透明预览
            scene()->addItem(tempItem);
            return;
        }
        // 添加起始点
        const auto currentPos = CanvasPointF(mapToScene(event->pos()));
        tempItem->setPoint(currentPos);
        tempItem->addPoint(currentPos);
        return;
    }
    // 结束绘制
    if (event->button() == Qt::RightButton && m_isDrawing) {
        m_isDrawing = false;
        auto finalPath = tempItem;
        scene()->removeItem(tempItem);
        tempItem = nullptr;
        finalPath->removePoint(-1); // 删除最后一个点
        finalPath->setFlag(QGraphicsItem::ItemIsSelectable, true);
        finalPath->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

        finalPath->setOnClickCallback([=]() {
            selectItem(finalPath);
        });
        addItem(finalPath);
    }
}

void CanvasSpace::onPathMouseMove(QMouseEvent *event) {
    if (!m_isDrawing || !tempItem) return;
    auto currentPos = CanvasPointF(mapToScene(event->pos()));
    tempItem->setPoint(currentPos);
    QGraphicsView::mouseMoveEvent(event);
}

void CanvasSpace::onPathMouseRelease(QMouseEvent *event) {
}

void CanvasSpace::onLinkMousePress(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;

        // 获取点击点/吸附点
        CanvasPointF createPoint = highlightHandler
                                       ? CanvasPointF(highlightHandler->getNearestPoint(mapToScene(event->pos())))
                                       : CanvasPointF(mapToScene(event->pos()));

        // 创建临时预览图元
        QRectF rect(createPoint, QSizeF(0, 0));
        tempItem = getSelectedTool()->itemDriver->newElement(rect);

        // 高亮图元添加链接点
        const auto firstLinkPoint = tempItem->getSelfLinkPoint(0);
        if (highlightedItem)
            highlightedItem->addObverseLinkPoint(firstLinkPoint);

        // 设置临时图元属性
        tempItem->setZValue(1000); // 置于顶层
        tempItem->setOpacity(0.5); // 半透明预览
        tempItem->setAcceptHoverEvents(false); // 禁用自身 hover 事件

        // 添加到场景
        scene()->addItem(tempItem);
    }
}

void CanvasSpace::onLinkMouseMove(QMouseEvent *event) {
    if (m_isDrawing) {
        if (!tempItem) return;
        CanvasPointF currentPoint(mapToScene(event->pos()));
        tempItem->setSelfLinkPoint(currentPoint);
    }

    QPointF scenePos = mapToScene(event->pos());
    QGraphicsItem *clickedItem = scene()->itemAt(scenePos, QTransform());

    // 如果点击的是空白区域
    if (!clickedItem) {
        scene()->clearSelection(); // 清除所有图元选中状态
        highlightItem(nullptr);
        return;
    }
    // 特殊处理，正在连接时
    if (!tempItem) return;

    QGraphicsSceneHoverEvent hoverEvent(QEvent::GraphicsSceneHoverMove);
    hoverEvent.setScenePos(scenePos);
    QApplication::sendEvent(scene(), &hoverEvent);
}

void CanvasSpace::onLinkMouseRelease(QMouseEvent *event) {
    if (!m_isDrawing || !tempItem) {
        return;
    }
    auto lastPoint = highlightHandler
                         ? CanvasPointF(highlightHandler->getNearestPoint(mapToScene(event->pos())))
                         : CanvasPointF(mapToScene(event->pos()));

    tempItem->setSelfLinkPoint(lastPoint);
    const auto lastLinkPoint = tempItem->getSelfLinkPoint();
    if (highlightedItem) {
        highlightedItem->addObverseLinkPoint(lastLinkPoint);
    }

    auto finalLink = createItem(getSelectedTool()->itemDriver,
                                tempItem->boundingRect(),
                                tempItem->parentItem());
    // 移交链接点
    auto pointsList = QList<QSharedPointer<CanvasPointF> >();
    for (auto &weakPtr: tempItem->getSelfLinkPoints()) {
        if (auto sharedPtr = weakPtr.toStrongRef()) {
            pointsList.append(sharedPtr);
        }
    }
    finalLink->acceptSelfLinkPoints(pointsList);
    scene()->removeItem(tempItem);
    delete tempItem;
    tempItem = nullptr;
    m_isDrawing = false;
    addItem(finalLink);
}

void CanvasSpace::mousePressEvent(QMouseEvent *event) {
    switch (getSelectedTool()->type) {
        case ToolType::SELECT:
            onSelectMousePress(event);
            break;
        case ToolType::SHAPE:
            onShapeMousePress(event);
            break;
        case ToolType::PATH:
            onPathMousePress(event);
            break;
        case ToolType::LINK:
            onLinkMousePress(event);
            break;
        default:
            break;
    }
    QGraphicsView::mousePressEvent(event);
}

void CanvasSpace::mouseMoveEvent(QMouseEvent *event) {
    switch (getSelectedTool()->type) {
        case ToolType::SELECT:
            onSelectMouseMove(event);
            break;
        case ToolType::SHAPE:
            onShapeMouseMove(event);
            break;
        case ToolType::PATH:
            onPathMouseMove(event);
            break;
        case ToolType::LINK:
            onLinkMouseMove(event);
            break;
        default:
            break;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void CanvasSpace::mouseReleaseEvent(QMouseEvent *event) {
    switch (getSelectedTool()->type) {
        case ToolType::SELECT:
            break;
        case ToolType::SHAPE:
            onShapeMouseRelease(event);
            break;
        case ToolType::PATH:
            onPathMouseRelease(event);
            break;
        case ToolType::LINK:
            onLinkMouseRelease(event);
            break;
        default:
            break;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CanvasSpace::wheelEvent(QWheelEvent *event) {
    // 计算缩放方向（1为放大，-1为缩小）
    if (!isZooming) {
        QGraphicsView::wheelEvent(event);
        return;
    }

    const qreal zoomInFactor = 1.1;
    const qreal zoomOutFactor = 0.9;
    qreal zoomFactor = (event->angleDelta().y() > 0) ? zoomInFactor : zoomOutFactor;
    applyZoom(zoomFactor, event->position());
    event->accept();
}

// 执行实际缩放操作
void CanvasSpace::applyZoom(qreal zoomFactor, QPointF mousePos) {
    // 计算新缩放系数
    qreal newZoom = m_currentZoom * zoomFactor;
    if (newZoom != m_currentZoom) {
        scale(zoomFactor, zoomFactor);
        m_currentZoom = newZoom;
    }
}

void CanvasSpace::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        isZooming = true;
    }
    switch (getSelectedTool()->type) {
        case ToolType::SELECT:
            onSelectKeyPress(event);
            break;
        case ToolType::SHAPE:
            break;
        case ToolType::PATH:
            break;
        case ToolType::LINK:
            break;
        default:
            break;
    }
    // QGraphicsView::keyPressEvent(event);
}

void CanvasSpace::keyReleaseEvent(QKeyEvent *event) {
    isZooming = false;
    QGraphicsView::keyReleaseEvent(event);
}
