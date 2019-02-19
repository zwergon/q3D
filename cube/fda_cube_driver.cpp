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

    Cube* cube = nullptr;
    int size_of = 0;
    switch(type){
    case SAVEF3UC:
        cube = new CubeUC;
        size_of = sizeof(quint8);
        break;
    case SAVEF3F:
        cube = new CubeF;
        size_of = sizeof(float);
        break;
    default:
        qCritical() << "type " << type << "is not readable !";
        return nullptr;
    }

    CubeModel* cube_model = new CubeModel();
    cube_model->setCube(cube);
    cube_model->setObjectName(fi.baseName());
    cube_model->setDriver(this);
    cube_model->cube().setSize(nx, ny, nz);

    long size = nx*ny*nz*size_of;

    QSharedMemory sharedMemory(QUuid::createUuid().toString());
    sharedMemory.create(size);

    sharedMemory.lock();
    stream.readRawData(static_cast<char*>(sharedMemory.data()), size);
    sharedMemory.unlock();

    cube_model->cube().attach(sharedMemory);

    cube_model->update();

    return cube_model;
}

void FdaCubeDriver::save( const Model& model, const QString& filename ){

    const CubeModel* cube_model = dynamic_cast<const CubeModel*>(&model);

    const Cube& cube = cube_model->cube();

    auto type = SAVEF3UC;
    if ( dynamic_cast<const CubeF*>(&cube) != nullptr ){
        type = SAVEF3F;
    }

    QFile file( filename );
    if (!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "unable to open " << filename << endl ;
        return;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    stream << type << cube.getNx() << cube.getNy() << cube.getNz();
    stream.writeRawData(static_cast<const char*>(cube.data()), cube.byteSize());


}

}
