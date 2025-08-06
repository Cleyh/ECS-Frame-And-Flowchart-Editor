//
// Created by unbd_ on 2025/5/5.
//

#ifndef CLASSICLINK_H
#define CLASSICLINK_H
#include "IPlugin.h"

class FEPLUGINS_API ClassicLink : public QObject, public IPlugin {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID "cn.mcug.flowchart_editor.plugins/1.0");
    Q_INTERFACES(IPlugin);

public:
    ClassicLink();
    bool Init() override;
    void Unload() override;
    void connectSignal(std::function<void()> callback, QWidget *parent) override;
    CanvasElement *createElement(const QRectF &, QGraphicsItem *) override;
    ~ClassicLink() override;
};

#endif //CLASSICLINK_H
