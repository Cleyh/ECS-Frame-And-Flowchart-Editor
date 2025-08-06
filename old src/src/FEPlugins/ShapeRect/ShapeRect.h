#ifndef IMAGECONTAINER_H
#define IMAGECONTAINER_H
#include "IPlugin.h"


class FEPLUGINS_API ShapeRect : public QObject, public IPlugin {
    Q_OBJECT;
    Q_PLUGIN_METADATA(IID "cn.mcug.flowchart_editor.plugins/1.0");
    Q_INTERFACES(IPlugin);

public:
    ShapeRect();
    ~ShapeRect() override;
    bool Init() override;
    void Unload() override;
    void connectSignal(std::function<void()> callback, QWidget *parent) override;
    CanvasElement *createElement(const QRectF &, QGraphicsItem *) override;
};


#endif //IMAGECONTAINER_H
