#include "ShapeSmile.h"

#include "SmileItem.h"

ShapeSmile::ShapeSmile()
    : IPlugin("ShapeSmile",
              "1.0",
              new QAction("Draw Smile"),
              ToolType::SHAPE) { ; }

ShapeSmile::~ShapeSmile() {
    if (action) delete action;
}

bool ShapeSmile::Init() {
    action->setIcon(QIcon(":/drawsmile_icon.svg"));
    qDebug() << "ShapeSmile::Init";
    return true;
}

void ShapeSmile::Unload() {
    qDebug() << "ShapeSmile::Unload";
}

void ShapeSmile::connectSignal(std::function<void()> callback,
                               QWidget *parent) {
    connect(action, &QAction::triggered, parent, [=]() {
        callback();
        qDebug() << "Draw Smile action triggered";
    });
}

CanvasElement *ShapeSmile::createElement(const QRectF &rect, QGraphicsItem *parent) {
    return new SmileItem(rect, parent);
}
