#ifndef DRIVERS_PLUGIN_H
#define DRIVERS_PLUGIN_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/plugins/plugin_collection.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT DriversPlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::DriverInterface)

public:
    explicit DriversPlugin(QObject* parent = nullptr);

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

class ActionsPlugin : public QObject, public ActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::ActionInterface)

public:
    explicit ActionsPlugin(QObject *parent = nullptr);
    virtual QList<PluginAction*> getActions( QObject* parent ) const;
    virtual QString name() const { return "Cube"; }
};

class DriversPluginCollection : public PluginCollection {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.DriverInterface" FILE "driversplugin.json")
    Q_INTERFACES(Q3D::CollectionInterface)

public:
    explicit DriversPluginCollection( QObject* parent = nullptr );

    virtual void start() override;
    virtual void end() override;
};

}

#endif // DRIVERS_PLUGIN_H
