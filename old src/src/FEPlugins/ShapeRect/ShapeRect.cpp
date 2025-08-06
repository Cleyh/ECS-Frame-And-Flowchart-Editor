#include "ShapeRect.h"

#include "RectItem.h"

ShapeRect::ShapeRect()
    : IPlugin("ImageContainer",
              "1.0",
              new QAction("Image Container"),
              ToolType::SHAPE) {
}

ShapeRect::~ShapeRect() {
    if (action) delete action;
}

bool ShapeRect::Init() {
    action->setIcon(QIcon(":/rect_icon.svg"));
    qDebug() << "ImageContainer::Init";
    return true;
}

void ShapeRect::Unload() {
}

void ShapeRect::connectSignal(const std::function<void()> callback, QWidget *parent) {
    connect(action, &QAction::triggered, parent, [=]() {
        callback();
        qDebug() << "Path Broken Line action triggered";
    });
}

CanvasElement *ShapeRect::createElement(const QRectF &rect, QGraphicsItem *parent) {
    return new RectItem(rect, parent);
}
