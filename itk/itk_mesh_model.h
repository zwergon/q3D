#ifndef ITKMESHMODEL_H
#define ITKMESHMODEL_H

#include <q3D/itk/itk_global.h>

#include <itkMesh.h>
#include <itkMatrix.h>

#include <q3D/model/model.h>

namespace Q3D {

using MeshType = itk::Mesh<float, 3>;
using PointIdentifier = MeshType::PointIdentifier;
using PointType = MeshType::PointType;
using CellType = MeshType::CellType;
using PointsContainerPointer = MeshType::PointsContainerPointer;
using CellIterator = MeshType::CellsContainerConstIterator;
using PointIdIterator = CellType::PointIdIterator;
using Matrix = itk::Matrix<float, 3>;

class ITKSHARED_EXPORT ItkMeshModel : public Model
{
    Q_OBJECT

public:
    ItkMeshModel();

    MeshType::Pointer getMesh() const;
    void setMesh(MeshType::Pointer mesh);

    PointType transformed( const PointType& pt ) const;

    virtual void update() override;
    virtual QString tooltip() const override;

private:
    MeshType::Pointer mesh_;
    Matrix matrix_;

};

inline void ItkMeshModel::setMesh(MeshType::Pointer mesh){
    mesh_ = mesh;
}

inline MeshType::Pointer ItkMeshModel::getMesh() const {
    return mesh_;
}

inline PointType ItkMeshModel::transformed(const PointType &pt) const {
    return matrix_ * pt;
}

}

#endif // ITKMESHMODEL_H
