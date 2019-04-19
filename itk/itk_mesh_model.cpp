#include "itk_mesh_model.h"

namespace Q3D {

ItkMeshModel::ItkMeshModel()
{
     matrix_[0][0] = 0;
     matrix_[0][1] = 0;
     matrix_[0][2] = -1;

     matrix_[1][0] = 0;
     matrix_[1][1] = -1;
     matrix_[1][2] = 0;

     matrix_[2][0] = -1;
     matrix_[2][1] = 0;
     matrix_[2][2] = 0;

}


void ItkMeshModel::update(){

    using BoundingBoxType = itk::BoundingBox< PointIdentifier, 3, float >;

    BoundingBoxType::Pointer boundingBox = BoundingBoxType::New();
    boundingBox->SetPoints(mesh_->GetPoints());
    boundingBox->ComputeBoundingBox();

    PointType min = transformed(boundingBox->GetMinimum());
    PointType max = transformed(boundingBox->GetMaximum());

    for( int i = 0; i<3; i++ ){
        min_[i] = min[i];
        max_[i] = max[i];
    }

    Model::update();

}

}
