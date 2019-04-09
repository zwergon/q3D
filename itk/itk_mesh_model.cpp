#include "itk_mesh_model.h"

namespace Q3D {

ItkMeshModel::ItkMeshModel()
{

}


void ItkMeshModel::update(){

    using BoundingBoxType = itk::BoundingBox< PointIdentifier, 3, float >;

    BoundingBoxType::Pointer boundingBox = BoundingBoxType::New();
    boundingBox->SetPoints(mesh_->GetPoints());
    boundingBox->ComputeBoundingBox();

    PointType min = boundingBox->GetMinimum();
    PointType max = boundingBox->GetMaximum();

    for( int i = 0; i<3; i++ ){
        min_[i] = min[i];
        max_[i] = max[i];
    }

    Model::update();

}

}
