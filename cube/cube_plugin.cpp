#include "cube_plugin.h"

#include <QDebug>

#include <q3D/cube/fda_cube_driver.h>

#ifdef WITH_MONGO
#include <mongoc/mongoc.h>
#include <q3D/cube/cube_load_mongo_dlg.h>
#endif

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

    return nullptr;
}

/**********************************************/

CubePluginAction::CubePluginAction(QObject *parent) : QObject(parent)
{
}

QMenu* CubePluginAction::tools() const {
    QMenu* cubeMenu = new QMenu("Cube");
#ifdef WITH_MONGO
    cubeMenu->addAction("load Mongo", this, &CubePluginAction::onCubeLoadMongo );
#endif

    return cubeMenu;
}

void CubePluginAction::onCubeLoadMongo(){
#ifdef WITH_MONGO
    CubeLoadMongoDlg dlg;
    dlg.exec();
#endif
}

/**********************************************/

CubePluginCollection::CubePluginCollection(QObject *parent) : QObject(parent){
}

void CubePluginCollection::start(){
#ifdef WITH_MONGO
   mongoc_init();
#endif
}

QList<QObject*> CubePluginCollection::plugins(){
    return QList<QObject*>{ new CubePlugin, new CubePluginAction};
}

void CubePluginCollection::end(){
#ifdef WITH_MONGO
    mongoc_cleanup();
#endif
}

}
