#include "ShapeCircle.h"

#include "CircleItem.h"

ShapeCircle::ShapeCircle()
    : IPlugin("ShapeCircle",
              "1.0",
              new QAction("Draw Circle"),
              ToolType::SHAPE) {
}

ShapeCircle::~ShapeCircle() {
    if (action) delete action;
}

bool ShapeCircle::Init() {
    action->setIcon(QIcon(":/drawcircle_icon.svg"));
    qDebug() << "DrawCircle::Init";
    return true;
}

void ShapeCircle::Unload() {
    qDebug() << "DrawCircle::Unload";
}

void ShapeCircle::connectSignal(std::function<void()> callback,
                                QWidget *parent) {
    connect(action, &QAction::triggered, parent, [=]() {
        callback();
        qDebug() << "Draw Circle action triggered";
    });
}

CanvasElement *ShapeCircle::createElement(const QRectF &rect, QGraphicsItem *parent) {
    return new CircleItem(rect, parent);
}
