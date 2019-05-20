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


void CubeModel::popupMenu(QMenu* parent){
}




}
