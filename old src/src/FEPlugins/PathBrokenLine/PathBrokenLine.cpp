#include "PathBrokenLine.h"

#include "BrokenLineItem.h"

PathBrokenLine::PathBrokenLine()
    : IPlugin("PathBrokenLine",
              "1.0",
              new QAction("Path Broken Line"),
              ToolType::PATH) { ; }

PathBrokenLine::~PathBrokenLine() {
    if (action) delete action;
}

bool PathBrokenLine::Init() {
    action->setIcon(QIcon(":/brokenline_icon.svg"));
    qDebug() << "PathBrokenLine::Init";
    return true;
}

void PathBrokenLine::Unload() {
    qDebug() << "PathBrokenLine::Unload";
}

void PathBrokenLine::connectSignal(std::function<void()> callback,
                                   QWidget *parent) {
    connect(action, &QAction::triggered, parent, [=]() {
        callback();
        qDebug() << "Path Broken Line action triggered";
    });
}

CanvasElement *PathBrokenLine::createElement(const QRectF &rect,
                                             QGraphicsItem *parent) {
    return new BrokenLineItem(rect, parent);
}
