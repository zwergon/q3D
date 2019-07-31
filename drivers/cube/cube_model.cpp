#include "cube_model.h"


#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_driver.h>

#include <QFileInfo>
#include <QDir>


namespace Q3D {

CubeModel::CubeModel(){
}

CubeModel::~CubeModel(){
    delete cube_;
}

void CubeModel::update(){

    const double* ord = cube_->origin();
    for( int i=0; i<3; i++ ){
        min_[i] = ord[i];
    }

    max_[0] = cube_->ijk2xyz( cube_->nx(), 0 );
    max_[1] = cube_->ijk2xyz( cube_->ny(), 1 );
    max_[2] = cube_->ijk2xyz( cube_->nz(), 2 );

    Model::update();

}


QString CubeModel::tooltip() const {

    char tooltip[] = "<html><head/><body>" \
                "<p><b>dim:</b> %1 %2 %3 </p>" \
                "<p><b>pixsize:</b> %4 %5 %6 </p>" \
                "<p><b>orig:</b> %7 %8 %9 </p>" \
                "</body></html>";
    const uint32_t* dim = cube_->dim();
    const double* pixsize = cube_->pixelSize();
    const double* orig = cube_->origin();
    return QString(tooltip)
            .arg(dim[0]).arg(dim[1]).arg(dim[2])
            .arg(pixsize[0]).arg(pixsize[1]).arg(pixsize[2])
            .arg(orig[0]).arg(orig[1]).arg(orig[2]);
}




}
