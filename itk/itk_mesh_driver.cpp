#include "itk_mesh_driver.h"

#include "itkMeshFileReader.h"
#include "itkMeshFileWriter.h"

#include <q3D/model/model_open_info.h>

#include <q3D/itk/itk_mesh_model.h>
#include <q3D/itk/itk_mesh_renderer.h>

#include <QFileInfo>

namespace Q3D {


typedef itk::MeshFileReader<MeshType> ReaderType;

ItkMeshDriver::ItkMeshDriver()
{
    setDescription("ItkMeshDriver");
    renderer_factory_.registerFactory<ItkMeshRenderer>( "ItkMesh" );
}


Model* ItkMeshDriver::open( const ModelOpenInfo& openInfo){
    QString fileName;
    try {
        const FileModelOpenInfo& fmoi = dynamic_cast<const FileModelOpenInfo&>(openInfo);
        fileName = fmoi.fileName();
    }
    catch(bad_cast){
        return nullptr;
    }

    QFileInfo fi(fileName);
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( fi.absoluteFilePath().toStdString() );
    reader->Update();


    MeshType::Pointer mesh = reader->GetOutput();
    ItkMeshModel* mesh_model = new ItkMeshModel();
    mesh_model->setMesh(mesh);
    mesh_model->setObjectName(fi.baseName());
    mesh_model->setDriver(this);
    mesh_model->update();

    return mesh_model;

}

void ItkMeshDriver::save( const Model& model, const QString& filename  ){

}

}
