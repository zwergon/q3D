#include "drivers_plugin.h"

#include "cpgmesh/cpgmesh_driver.h"
#include "mesh/mesh_driver.h"

namespace Q3D {

/**********************************************/

DriversPlugin::DriversPlugin(QObject* parent ) : QObject(parent)
{
    drivers_.append( new CpgMeshDriver );
    drivers_.append( new MeshDriver );
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

DriversPluginCollection::DriversPluginCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = new DriversPlugin(this);
}


}


