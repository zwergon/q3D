#include "cube_model.h"

namespace Q3D {

CubeModel::CubeModel(){
}

void CubeModel::update(){

    for( int i=0; i<3; i++ ){
        min_[i] = 0;
    }

    max_[0] = cube_.getNx();
    max_[1] = cube_.getNy();
    max_[2] = cube_.getNz();

    Model::update();

}


void CubeModel::popupMenu(QMenu* parent){
    QMenu* menu = new QMenu("Cube", parent);
    if ( nullptr != menu ){
        menu->addAction( "Launch", this, &CubeModel::onCubeLaunchAction);
    }
    parent->addMenu(menu);
}

void CubeModel::onCubeLaunchAction(){
    qDebug() << "onCubeLauchAction";
}

}
