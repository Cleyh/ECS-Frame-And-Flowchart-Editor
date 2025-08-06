//
// Created by unbd_ on 2025/5/5.
//
#include "ClassicLink.h"

#include "LinkItem.h"

bool ClassicLink::Init() {
    action->setIcon(QIcon(":/link_icon.svg"));
    qDebug() << "ClassicLink::Init";
    return true;
}

void ClassicLink::Unload() {
}

void ClassicLink::connectSignal(std::function<void()> callback, QWidget *parent) {
    connect(action, &QAction::triggered, parent, [=]() {
        callback();
        qDebug() << "Classic Link action triggered";
    });
}

CanvasElement *ClassicLink::createElement(const QRectF &rect, QGraphicsItem *parent) {
    return new LinkItem(rect, parent);
}

ClassicLink::~ClassicLink() {
    if (action) delete action;
}

ClassicLink::ClassicLink()
    : IPlugin(
        "ClassicLink",
        "1.0",
        new QAction("Classic Link"),
        ToolType::LINK) {
}
