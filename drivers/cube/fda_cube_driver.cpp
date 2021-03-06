#include "fda_cube_driver.h"

#include <QDebug>
#include <QFileInfo>

#include <q3D/cube/cube.h>
#include <q3D/cube/io/fda.h>

#include <q3D/drivers/cube/cube_model.h>
#include <q3D/drivers/cube/cube_renderer.h>

namespace Q3D {

FdaCubeDriver::FdaCubeDriver()
{
    setDescription("FdaCubeDriver");
    renderer_factory_.registerFactory<CubeRenderer>( "Cube" );
}

bool FdaCubeDriver::canHandle(Model* model) const{
    return dynamic_cast<CubeModel*>(model) != nullptr;
}

ModelOpenInfo* FdaCubeDriver::openInfo() const {
    FileModelOpenInfo* fmoi = new FileModelOpenInfo();
    fmoi->setExtension("fda");
    return fmoi;
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

    Cube* cube = FDA::read(fi.absoluteFilePath().toStdString());
    if ( cube == nullptr ){
        return nullptr;
    }

    CubeModel* cube_model = new CubeModel();
    cube_model->setCube(cube);
    cube_model->setObjectName(fi.baseName());
    cube_model->setDriver(this);
    cube_model->update();

    return cube_model;
}

void FdaCubeDriver::save( const Model& model, const ModelOpenInfo& moi ){

    const CubeModel* cube_model = dynamic_cast<const CubeModel*>(&model);

    const FileModelOpenInfo* fmoi = static_cast<const FileModelOpenInfo*>(&moi);

    FDA::write(cube_model->cube(), fmoi->fileName().toStdString());

}

}
