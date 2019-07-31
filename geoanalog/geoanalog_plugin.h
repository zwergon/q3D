#ifndef GEOANALOG_PLUGIN_H
#define GEOANALOG_PLUGIN_H


#include <q3D/geoanalog/geoanalog_global.h>

#include <q3D/plugins/plugin_collection.h>


class GeoanalogDriverPlugin : public Q3D::DriverInterface {
    Q_OBJECT
    Q_INTERFACES(Q3D::DriverInterface)
public:
    explicit GeoanalogDriverPlugin(QObject* parent);
};

class GeoanalogActionsPlugin : public QObject, public Q3D::ActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::ActionInterface)

public:
    explicit GeoanalogActionsPlugin(QObject *parent = nullptr);
    virtual QList<Q3D::PluginAction*> getActions( QObject* parent ) const;
    virtual QString name() const { return "Geoanalog"; }
};

class GeoanalogPluginCollection : public Q3D::PluginCollection {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.DriverInterface" FILE "geoanalog_plugin.json")
    Q_INTERFACES(Q3D::CollectionInterface)

public:
    explicit GeoanalogPluginCollection( QObject* parent = nullptr );

    virtual void start() override;
    virtual void end() override;

};


#endif // GEOANALOG_PLUGIN_H
