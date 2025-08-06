//
// Created by unbd_ on 2025/5/4.
//

#ifndef PATHBROKENLINE_H
#define PATHBROKENLINE_H
#include "IPlugin.h"

class FEPLUGINS_API PathBrokenLine : public QObject, public IPlugin {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID "cn.mcug.flowchart_editor.plugins/1.0");
    Q_INTERFACES(IPlugin);

public:
    PathBrokenLine();
    ~PathBrokenLine() override;
    bool Init() override;
    void Unload() override;
    void connectSignal(std::function<void()> callback, QWidget *parent) override;
    CanvasElement *createElement(const QRectF &, QGraphicsItem *) override;
};


#endif //PATHBROKENLINE_H
