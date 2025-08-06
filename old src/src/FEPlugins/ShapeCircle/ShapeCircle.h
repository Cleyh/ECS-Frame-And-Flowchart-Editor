#pragma once

#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include "IPlugin.h"
#include "../../DataModels/Basic/BasicPoint.h"

class FEPLUGINS_API ShapeCircle
        : public QObject, public IPlugin {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID "cn.mcug.flowchart_editor.plugins/1.0");
    Q_INTERFACES(IPlugin);

public:
    ShapeCircle();
    ~ShapeCircle() override;
    bool Init() override;
    void Unload() override;
    void connectSignal(std::function<void()> callback, QWidget *parent = nullptr) override;
    CanvasElement *createElement(const QRectF &rect,
                                 QGraphicsItem *parent) override;
};
#endif // DRAWCIRCLE_H
