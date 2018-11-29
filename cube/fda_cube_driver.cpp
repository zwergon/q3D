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


Model* FdaCubeDriver::open( const QString& fileName){

    QFileInfo fi(fileName);
    if ( !fi.exists() || (fi.suffix() != "fda")){
        qDebug() << description() << " is not the good driver\n";
        return 0;
    }

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "unable to open " << fileName << endl ;
        return  0;
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

    quint8* buffer = new quint8[size];
    for( int i=0; i<size; i++ ){
        stream >> buffer[i];
    }

    cube_model->cube().setData(buffer);

    //delete [] buffer;

    cube_model->update();

    return cube_model;
}

void FdaCubeDriver::save( const Model& model, const QString& filename ){

}

}
