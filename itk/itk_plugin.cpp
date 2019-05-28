#include "itk_plugin.h"

#include "itkVTKPolyDataMeshIOFactory.h"

#include <q3D/itk/itk_mesh_driver.h>
#include <q3D/itk/itk_cube_actions.h>

namespace Q3D {


/**********************************************/

ItkDriversPlugin::ItkDriversPlugin(QObject* parent ) : QObject(parent)
{
    drivers_.append(new ItkMeshDriver);
}

QStringList ItkDriversPlugin::drivers() const{
    QStringList drivers;
    QListIterator<ModelDriver*> itd( drivers_ );
    while( itd.hasNext() ){
        ModelDriver* driver = itd.next();
        drivers.append( driver->description() );
    }

    return drivers;
}

ModelDriver* ItkDriversPlugin::driver( const QString& key ){
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

ItkActionPlugin::ItkActionPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> ItkActionPlugin::getActions( QObject* parent ) const {

    QList<PluginAction*> actions;
    actions.append(new MedianFilterAction(parent));
    actions.append(new ExtractSurfAction(parent));
    return actions;

}



/**********************************************/

ItkPluginCollection::ItkPluginCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = new ItkDriversPlugin(this);
    action_interface_ = new ItkActionPlugin(this);
}

void ItkPluginCollection::start() {
    itk::VTKPolyDataMeshIOFactory::RegisterOneFactory();
}




}
