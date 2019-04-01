#include "drivers_plugin.h"

#include "cpgmesh/cpgmesh_driver.h"
#include "mesh/mesh_driver.h"
#include "cube/fda_cube_driver.h"
#ifdef WITH_MONGO
#include <mongoc/mongoc.h>
#include "cube/mongo_cube_driver.h"
#include "cube/mongo_load_action.h"
#endif
#include "cube/cube_actions.h"

namespace Q3D {

/**********************************************/

DriversPlugin::DriversPlugin(QObject* parent ) : QObject(parent)
{
    drivers_.append( new CpgMeshDriver );
    drivers_.append( new MeshDriver );
    drivers_.append(new FdaCubeDriver);
   #ifdef WITH_MONGO
       drivers_.append(new MongoCubeDriver);
   #endif
}

QStringList DriversPlugin::drivers() const{
    QStringList drivers;
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        drivers.append( driver->description() );
    }

    return drivers;
}

ModelDriver* DriversPlugin::driver( const QString& key ){
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        if ( key == driver->description() ){
            return driver;
        }
    }

    return 0;
}

/**********************************************/

ActionsPlugin::ActionsPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> ActionsPlugin::getActions(QObject* parent) const {
    QList<PluginAction*> actions;
#ifdef WITH_MONGO
    actions.append(new MongoLoadAction(parent));
#endif
    actions.append(new CubeWaveAction(parent));
    return actions;
}


/**********************************************/

DriversPluginCollection::DriversPluginCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = new DriversPlugin(this);
    action_interface_ = new ActionsPlugin(this);
}



/**********************************************/

void DriversPluginCollection::start(){
#ifdef WITH_MONGO
   mongoc_init();
#endif
}

void DriversPluginCollection::end(){
#ifdef WITH_MONGO
    mongoc_cleanup();
#endif
}



}


