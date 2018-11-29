#include "mesh_model.h"


namespace Q3D {

MeshModel::MeshModel(){
}

void
MeshModel::update(){

    mesh_.update_bounding_box();

    const Box3d& bb = mesh_.box();
    for( int i=0; i<3; i++ ){
        min_[i] = (float)bb[0][i];
        max_[i] = (float)bb[1][i];
    }

    Model::update();

}

}
