#include "cube_plugin.h"

#include <q3D/cube/fda_cube_driver.h>

namespace Q3D {

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

}
