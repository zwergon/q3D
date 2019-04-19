#include "nifti_cube_driver.h"

#include <QDebug>
#include <QFileInfo>

#include <string>

#include <q3D/cube/cube.h>
#include <q3D/cube/nifti/nifti1_io.h>

#include <q3D/drivers/cube/cube_model.h>
#include <q3D/drivers/cube/cube_renderer.h>



namespace Q3D {



NiftiCubeDriver::NiftiCubeDriver()
{
    setDescription("NiftiCubeDriver");
    renderer_factory_.registerFactory<CubeRenderer>( "Cube" );
}

bool NiftiCubeDriver::canHandle(Model *model) const {
    return dynamic_cast<CubeModel*>(model) != nullptr;
}


ModelOpenInfo* NiftiCubeDriver::openInfo() const {
    FileModelOpenInfo* fmoi = new FileModelOpenInfo();
    fmoi->setExtension("nii");
    return fmoi;
}


Model* NiftiCubeDriver::open(const ModelOpenInfo& openInfo )
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
    if ( !fi.exists() || (fi.suffix() != "nii")){
        qDebug() << description() << " is not the good driver\n";
        return nullptr;
    }

    nifti_image* nim = nullptr;

    nim = nifti_image_read(fi.absoluteFilePath().toLatin1(), 1);
    if( nim == nullptr ) {
       qDebug() <<  "failed to read NIfTI image from " << fileName;
       return nullptr;
    }

    Cube* cube = nullptr;
    switch(nim->datatype){
    case DT_FLOAT:
        cube = Cube::create(Cube::FLOAT, true);
        break;
    case DT_UINT32:
        cube = Cube::create(Cube::UINT32, true);
        break;
    case DT_UINT8:
        cube = Cube::create(Cube::UINT8, true);
        break;
    case DT_DOUBLE:
        cube = Cube::create(Cube::DOUBLE, true);
        break;
    default:
        qDebug() << "unable to create a Cube for type " << nim->datatype;
        return nullptr;
    }

    if ( nim->ndim != 3 ){
        qDebug() << "only handle Cube of dimension 3 ";
        return nullptr;
    }

    //nim->dim[0] stores the number of dimensions too.
    cube->allocate(nim->dim[3], nim->dim[2], nim->dim[1]);
    cube->setOrigin(nim->qoffset_z, nim->qoffset_y, nim->qoffset_x);
    cube->setPixelSize(nim->pixdim[3],  nim->pixdim[2], nim->pixdim[1]);

    memcpy( cube->data(), nim->data, cube->byteSize());

    CubeModel* cube_model = new CubeModel();
    cube_model->setCube(cube);
    cube_model->setObjectName(fi.baseName());
    cube_model->setDriver(this);
    cube_model->update();

    nifti_image_free(nim);

    return cube_model;

}

void NiftiCubeDriver::save( const Model& model, const ModelOpenInfo& moi ){


    const CubeModel* cube_model = dynamic_cast<const CubeModel*>(&model);
    if ( cube_model == nullptr ){
        return;
    }

    const FileModelOpenInfo* fmoi = static_cast<const FileModelOpenInfo*>(&moi);

    const Cube& cube = cube_model->cube();

    int dim[] = {3, cube.nz(), cube.ny(), cube.nx(), 0, 0, 0, 0};


    nifti_1_header* nhdr = nullptr;

    switch(cube.type()){
    case Cube::UINT8:
         nhdr = nifti_make_new_header(dim, DT_UINT8);
        break;
    case Cube::UINT32:
        nhdr = nifti_make_new_header(dim, DT_UINT32);
        break;
    case Cube::FLOAT:
        nhdr = nifti_make_new_header(dim, DT_FLOAT);
        break;
    case Cube::DOUBLE:
        nhdr = nifti_make_new_header(dim, DT_DOUBLE);
        break;
    default:
        qDebug() << "unable to save nifty this kind " << cube.type();
        break;
    }

    /* quaternion transformation is explained in
     * https://brainder.org/2012/09/23/the-nifti-file-format
     * */
    const double* pixdim = cube.pixelSize();
    nhdr->pixdim[0] = -1.; // will be nim->qfac for quaternion transformation.
    nhdr->pixdim[1] = pixdim[2];
    nhdr->pixdim[2] = pixdim[1];
    nhdr->pixdim[3] = pixdim[0];


    nhdr->qform_code = NIFTI_XFORM_ALIGNED_ANAT;
    const double* ori = cube.origin();
    nhdr->quatern_b = 0; //no rotation
    nhdr->quatern_c = 0; //no rotation
    nhdr->quatern_d = 0; //no rotation
    nhdr->qoffset_x = ori[2];
    nhdr->qoffset_y = ori[1];
    nhdr->qoffset_z = ori[0];

    nifti_image* nim = nifti_convert_nhdr2nim(*nhdr, fmoi->fileName().toLatin1());

    nim->data = (void*)cube.data();

    nifti_image_infodump(nim);

    nifti_image_write(nim);

    //to avoid deletion of data in cube
    nim->data = nullptr;

    if ( nhdr != nullptr ) free(nhdr);
    nifti_image_free(nim);

}

}
