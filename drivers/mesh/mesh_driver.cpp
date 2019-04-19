/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Francois
 *
 * $Id: Mesh_driver.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */

#include "mesh_driver.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <q3D/mesh/mesh.h>
#include <q3D/filters/gmsh_mesh_importer.h>
#include <q3D/filters/gmsh_mesh_exporter.h>
#include "mesh_renderer_menu.h"
#include "mesh_model.h"

#include "mesh_renderer.h"

namespace Q3D {

MeshDriver::MeshDriver(){
    setDescription("MeshDriver");
    renderer_factory_.registerFactory<MeshRenderer>( "Mesh" );
    RendererMenu::factory().registerFactory<MeshRendererMenu>( MeshRenderer::staticMetaObject.className() );
}

bool MeshDriver::canHandle(Model* model) const {
    return dynamic_cast<MeshModel*>(model) != nullptr;
}

ModelOpenInfo* MeshDriver::openInfo() const {
    FileModelOpenInfo* fmoi = new FileModelOpenInfo();
    fmoi->setExtension("gmsh");
    return fmoi;
}

Model *
MeshDriver::open( const ModelOpenInfo& openInfo )
{
    QString fileName;
    try {
        const FileModelOpenInfo& fmoi = dynamic_cast<const FileModelOpenInfo&>(openInfo);
        fileName = fmoi.fileName();
    }
    catch(bad_cast){
        return nullptr;
    }

    //create Core data
    MeshModel* model  = new MeshModel;
    model->setObjectName( fileName );
    model->setDriver( this );

    GmshMeshImporter importer;
    importer.set_filename( fileName );

    if ( !importer.create_mesh( &(model->mesh()) ) ){
        qDebug() << description() << " is not the good driver\n";
        delete model;
        return nullptr;
    }

    qDebug() << description() << " is the good driver\n";

    model->update();

    return model;
}

void
MeshDriver::save( const Model& model, const ModelOpenInfo& moi ){

    const MeshModel* mesh_model = static_cast<const MeshModel*>( &model );
    Mesh& mesh = const_cast<MeshModel*>(mesh_model)->mesh();

    const FileModelOpenInfo* fmoi = static_cast<const FileModelOpenInfo*>(&moi);

    GmshMeshExporter exporter;
    exporter.set_filename( fmoi->fileName() );
    exporter.to_file( &mesh );

}


}
