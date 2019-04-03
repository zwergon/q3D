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

    for( int i=0; i<3; i++ ){
        min_[i] = 0;
    }

    max_[0] = cube_->nx();
    max_[1] = cube_->ny();
    max_[2] = cube_->nz();

    Model::update();

}


void CubeModel::popupMenu(QMenu* parent){
}




}
