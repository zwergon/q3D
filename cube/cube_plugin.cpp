#include "cube_plugin.h"

#include <QDebug>

#include <q3D/cube/fda_cube_driver.h>
#ifdef WITH_MONGO
#include <mongoc/mongoc.h>
#include <q3D/cube/mongo_cube_driver.h>
#include <q3D/cube/mongo_load_action.h>
#endif

namespace Q3D {

/**********************************************/

CubeDriverPlugin::CubeDriverPlugin(QObject* parent) : QObject(parent)
{
    drivers_.append(new FdaCubeDriver);
#ifdef WITH_MONGO
    drivers_.append(new MongoCubeDriver);
#endif
}

QStringList CubeDriverPlugin::drivers() const{
    QStringList drivers;
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        drivers.append( driver->description() );
    }

    return drivers;
}

ModelDriver* CubeDriverPlugin::driver( const QString& key ){
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        if ( key == driver->description() ){
            return driver;
        }
    }

    return nullptr;
}

/**********************************************/

CubeActionPlugin::CubeActionPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> CubeActionPlugin::getActions() const {
    QList<PluginAction*> actions;
#ifdef WITH_MONGO
    actions.append(new MongoLoadAction);
#endif
    return actions;
}


/**********************************************/

CubePluginCollection::CubePluginCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = new CubeDriverPlugin(this);
    action_interface_ = new CubeActionPlugin(this);
}

void CubePluginCollection::start(){
#ifdef WITH_MONGO
   mongoc_init();
#endif
}

void CubePluginCollection::end(){
#ifdef WITH_MONGO
    mongoc_cleanup();
#endif
}

}
