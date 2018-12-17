#ifndef INTERFACES_H
#define INTERFACES_H

#include <q3D/plugins/plugins_global.h>

#include <QtPlugin>
#include <QMenu>


namespace Q3D {

class ModelDriver;
class PluginAction;

class PLUGINSSHARED_EXPORT PluginActionInterface {
public:
    virtual ~PluginActionInterface() {}

    virtual QMenu* tools() const = 0;
};


class PLUGINSSHARED_EXPORT DriverInterface {
public:
    virtual ~DriverInterface(){}

    virtual QStringList drivers() const = 0;
    virtual ModelDriver* driver( const QString& key ) = 0;
};

class PLUGINSSHARED_EXPORT PluginCollectionInterface {
public:
    virtual ~PluginCollectionInterface(){}

    virtual QList<QObject*> plugins() = 0;
};

}

Q_DECLARE_INTERFACE(Q3D::PluginActionInterface,
                     "fr.org.q3D.PluginActionInterface/1.0")

Q_DECLARE_INTERFACE(Q3D::DriverInterface,
                     "fr.org.q3D.DriverInterface/1.0")

Q_DECLARE_INTERFACE(Q3D::PluginCollectionInterface,
                     "fr.org.q3D.PluginCollectionInterface/1.0")


#endif // INTERFACES_H
