#ifndef PLUGIN_ACTION_H
#define PLUGIN_ACTION_H

#include <q3D/plugins/plugins_global.h>

#include <QObject>
#include <QAction>

namespace Q3D {

class Model;

class AbstractTool;

class PLUGINSSHARED_EXPORT PluginAction : public QObject
{
    Q_OBJECT

public:
    enum {
        MENU_ACTION,
        IO_ACTION,
        TOOL_ACTION
    };

public:
    virtual ~PluginAction();

    int getType() const { return type_; }

    QString getDescription() const { return description_; }
    QAction* getAction() const { return action_; }

    virtual bool canWorkOn(Model*) const;
    virtual bool execute(Model*);

signals:
    void activated( PluginAction* );

protected:
    explicit PluginAction(int type, QObject *parent = nullptr);

protected:
    int type_;
    QString description_;
    QAction* action_;

private slots:
    void on_action_triggered();
};




}

#endif // PLUGIN_ACTION_H
