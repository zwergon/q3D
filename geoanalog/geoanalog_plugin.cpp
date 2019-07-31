#include "geoanalog_plugin.h"

#ifdef WITH_MONGO
#include <mongoc/mongoc.h>
#endif
#include <QDebug>

#include <q3D/geoanalog/geoanalog_action.h>
#include <q3D/geoanalog/geoanalog_driver.h>

/**********************************************/

GeoanalogDriverPlugin::GeoanalogDriverPlugin(QObject* parent ) : DriverInterface(parent)
{
#ifdef WITH_MONGO
    qDebug() << "add GeoanalogDriver";
    drivers_.append(new GeoanalogDriver);
#endif
}


/**********************************************/

GeoanalogActionsPlugin::GeoanalogActionsPlugin(QObject *parent) : QObject(parent)
{
}

QList<Q3D::PluginAction*> GeoanalogActionsPlugin::getActions(QObject* parent) const {
    QList<Q3D::PluginAction*> actions;
#ifdef WITH_MONGO
    actions.append(new GeoanalogAction(parent));
#endif;
    return actions;
}


/**********************************************/

GeoanalogPluginCollection::GeoanalogPluginCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = new GeoanalogDriverPlugin(this);
    action_interface_ = new GeoanalogActionsPlugin(this);
}



/**********************************************/

void GeoanalogPluginCollection::start(){
#ifdef WITH_MONGO
   mongoc_init();
#endif
}

void GeoanalogPluginCollection::end(){
#ifdef WITH_MONGO
    mongoc_cleanup();
#endif
}
