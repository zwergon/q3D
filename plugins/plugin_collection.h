#ifndef INTERFACES_H
#define INTERFACES_H

#include <q3D/plugins/plugins_global.h>

#include <QObject>

#include <q3D/plugins/plugin_interfaces.h>

namespace Q3D {

class PLUGINSSHARED_EXPORT PluginCollection : public QObject, public CollectionInterface{
    Q_OBJECT
public:
    explicit PluginCollection(QObject* parent=nullptr);
    virtual ~PluginCollection();

    virtual void start() override;
    virtual ActionInterface* getActionPlugin() override;
    virtual DriverInterface* getDriverPlugin() override;
    virtual void end() override;

protected:
    ActionInterface* action_interface_;
    DriverInterface* driver_interface_;
};

}



#endif // INTERFACES_H
