#pragma once

#ifndef SHAPESMILE_H
#define SHAPESMILE_H

#include "IPlugin.h"

class FEPLUGINS_API ShapeSmile
        : public QObject, public IPlugin {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID "cn.mcug.flowchart_editor.plugins/1.0");
    Q_INTERFACES(IPlugin);

public:
    ShapeSmile();
    ~ShapeSmile() override;
    bool Init() override;
    void Unload() override;
    void connectSignal(std::function<void()> callback, QWidget *parent = nullptr) override;
    CanvasElement *createElement(const QRectF &rect,
                                 QGraphicsItem *parent) override;
};
#endif // SHAPESMILE_H
