#pragma once

#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <qobject.h>
#include <qaction.h>
#include <qwidget.h>
#include <qdebug.h>
#include <QtPlugin>
#include "CanvasBase/CanvasElement.h"
#include "QGraphicsEllipseItem"
#include "TypeDefine.h"
#include "Basic/BasicPoint.h"


#if defined(FEPLUGINS_EXPORTS)
#define FEPLUGINS_API Q_DECL_EXPORT
#else
    #define FEPLUGINS_API Q_DECL_IMPORT
#endif

using TypeDefine::ToolType;

struct PluginInfo {
    QString pluginName;
    QString pluginVersion;
};

class FEPLUGINS_API IPlugin {
public:
    explicit IPlugin(const QString &plugin_name = "ExamplePlugin",
                     const QString &plugin_version = "1.0",
                     QAction *action = new QAction("Example Action"),
                     const ToolType tool_type = ToolType::SELECT)
        : pluginName(plugin_name),
          pluginVersion(plugin_version),
          action(action),
          toolType(tool_type) { ; }

    virtual ~IPlugin() = default;
    virtual bool Init() = 0;
    virtual void Unload() = 0;
    virtual void connectSignal(std::function<void()> callback, QWidget *parent) = 0;
    virtual CanvasElement *createElement(const QRectF &, QGraphicsItem *) = 0;

    virtual PluginInfo GetPluginInfo() { return {pluginName, pluginVersion}; };
    virtual QAction *GetAction() { return action; };
    virtual ToolType GetToolType() { return toolType; };

protected:
    QString pluginName = "ExamplePlugin";
    QString pluginVersion = "1.0";
    QAction *action = new QAction("Example Action");
    ToolType toolType = ToolType::SELECT;
};

Q_DECLARE_INTERFACE(IPlugin, "cn.mcug.flowchart_editor.plugins/1.0")
#endif // IPLUGIN_H
