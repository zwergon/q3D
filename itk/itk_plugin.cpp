#include "itk_plugin.h"

#include "itkVTKPolyDataMeshIOFactory.h"

#include <q3D/itk/itk_mesh_driver.h>
#include <q3D/itk/itk_cube_actions.h>

namespace Q3D {


/**********************************************/

ItkDriversPlugin::ItkDriversPlugin(QObject* parent ) : DriverInterface(parent)
{
    drivers_.append(new ItkMeshDriver);
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
