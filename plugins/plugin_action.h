#ifndef PLUGIN_ACTION_H
#define PLUGIN_ACTION_H

#include <q3D/plugins/plugins_global.h>

#include <QObject>
#include <QAction>

namespace Q3D {


class AbstractTool;

class PLUGINSSHARED_EXPORT PluginAction : public QObject
{
    Q_OBJECT
public:
    explicit PluginAction(QObject *parent = nullptr);
    virtual ~PluginAction();

    virtual QString getDescription() const = 0;
};


class PLUGINSSHARED_EXPORT PluginIOAction : public PluginAction {
    Q_OBJECT

public:
    explicit PluginIOAction(QObject* parent = nullptr);

    virtual QAction* getAction() const = 0;
};

class PLUGINSSHARED_EXPORT PluginToolAction : public PluginAction {
    Q_OBJECT

public:
    explicit PluginToolAction(QObject* parent = nullptr);

    virtual AbstractTool* getTool() const = 0;
};


}

#endif // PLUGIN_ACTION_H
