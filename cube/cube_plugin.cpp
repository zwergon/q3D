#include "cube_plugin.h"

#include <q3D/cube/fda_cube_driver.h>

namespace Q3D {

/**********************************************/

CubePlugin::CubePlugin()
{
    drivers_.append( new FdaCubeDriver );
}

QStringList CubePlugin::drivers() const{
    QStringList drivers;
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        drivers.append( driver->description() );
    }

    return drivers;
}

ModelDriver* CubePlugin::driver( const QString& key ){
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

CubePluginAction::CubePluginAction(QObject *parent) : QObject(parent)
{
}

QStringList CubePluginAction::tools() const {
    return {"Launch"};
}

PluginAction* CubePluginAction::action( const QString& key ){
    return nullptr;
}

/**********************************************/

CubePluginCollection::CubePluginCollection(QObject *parent) : QObject(parent){
}

QList<QObject*> CubePluginCollection::plugins(){
    return QList<QObject*>{ new CubePlugin, new CubePluginAction};
}

}
