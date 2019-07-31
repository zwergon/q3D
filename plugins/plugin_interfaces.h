#ifndef PLUGIN_INTERFACES_H
#define PLUGIN_INTERFACES_H

#include <q3D/plugins/plugins_global.h>


#include <QtPlugin>

namespace Q3D {

class PluginAction;
class ModelDriver;

class PLUGINSSHARED_EXPORT ActionInterface {
public:
    virtual QString name() const = 0;
    virtual QList<PluginAction*> getActions( QObject* parent ) const = 0;
};


class PLUGINSSHARED_EXPORT DriverInterface : public QObject {
    Q_OBJECT
public:
    DriverInterface(QObject* parent =  nullptr);
    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

protected:
    QList<ModelDriver*> drivers_;
};

class PLUGINSSHARED_EXPORT CollectionInterface {
public:
    virtual void start() = 0;
    virtual ActionInterface* getActionPlugin() = 0;
    virtual DriverInterface* getDriverPlugin() = 0;
    virtual void end() =0;
};

}

Q_DECLARE_INTERFACE(Q3D::ActionInterface,
                     "fr.org.q3D.ActionInterface/1.0")

Q_DECLARE_INTERFACE(Q3D::DriverInterface,
                     "fr.org.q3D.DriverInterface/1.0")

Q_DECLARE_INTERFACE(Q3D::CollectionInterface,
                     "fr.org.q3D.CollectionInterface/1.0")

#endif // PLUGIN_INTERFACES_H
