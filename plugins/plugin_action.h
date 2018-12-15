#ifndef PLUGIN_ACTION_H
#define PLUGIN_ACTION_H

#include <q3D/plugins/plugins_global.h>

#include <QObject>

namespace Q3D {

class ModelManager;

class PLUGINSSHARED_EXPORT PluginAction : public QObject {
        Q_OBJECT
public:
    virtual QString name() const = 0;

public slots:
    virtual void execute( ModelManager* );
};

}

#endif // PLUGIN_ACTION_H
