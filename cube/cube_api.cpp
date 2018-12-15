#include "cube_api.h"

#include <QDebug>

#include <q3D/model/model_driver.h>
#include <q3D/model/model_driver_mgr.h>

#include <QSharedMemory>


using namespace Q3D;

bool get_cube( char* key, quint8* buffer, int nx, int ny, int nz ){

    QSharedMemory sharedCube(key);
    if ( sharedCube.attach() ){
        qDebug() << "load cube " << key << " with size " << nx*ny*nz;
        sharedCube.lock();
        memcpy(buffer, sharedCube.data(), nx*ny*nz);
        sharedCube.unlock();
        return true;
    }

    return false;

}
