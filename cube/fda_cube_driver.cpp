#include "fda_cube_driver.h"

#include <QDebug>
#include <QFileInfo>

#include <q3D/cube/cube.h>
#include <q3D/cube/cube_model.h>
#include <q3D/cube/cube_renderer.h>

namespace Q3D {

FdaCubeDriver::FdaCubeDriver()
{
    setDescription("FdaCubeDriver");
    renderer_factory_.registerFactory<CubeRenderer>( "Cube" );
}


Model* FdaCubeDriver::open(const ModelOpenInfo& openInfo )
{
    QString fileName;
    try {
        const FileModelOpenInfo& fmoi = dynamic_cast<const FileModelOpenInfo&>(openInfo);
        fileName = fmoi.fileName();
    }
    catch(bad_cast){
        return nullptr;
    }


    QFileInfo fi(fileName);
    if ( !fi.exists() || (fi.suffix() != "fda")){
        qDebug() << description() << " is not the good driver\n";
        return nullptr;
    }

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "unable to open " << fileName << endl ;
        return  nullptr;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    qint32 nx, ny, nz, type;

    stream >> type >> nx >> ny >> nz;

    CubeModel* cube_model = new CubeModel();
    cube_model->setObjectName(fileName);
    cube_model->setDriver(this);
    cube_model->cube().setSize(nx, ny, nz);

    long size = nx*ny*nz;

    QSharedMemory sharedMemory(QUuid::createUuid().toString());
    sharedMemory.create(size);

    sharedMemory.lock();
    quint8* buffer = static_cast<quint8*>(sharedMemory.data());
    for( int i=0; i<size; i++ ){
        stream >> buffer[i];
    }
    sharedMemory.unlock();

    cube_model->cube().attach(sharedMemory);

    cube_model->update();

    return cube_model;
}

void FdaCubeDriver::save( const Model& model, const QString& filename ){

}

}
