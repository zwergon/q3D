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

}
