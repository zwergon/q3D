#ifndef MODEL_MESH_H
#define MODEL_MESH_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/model.h>
#include <q3D/mesh/mesh.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT MeshModel : public Model
{
    Q_OBJECT

public:
    MeshModel();
    Mesh& mesh();

    virtual void update();

private:
    Mesh  mesh_;
};

inline Mesh& MeshModel::mesh() {
    return mesh_;
}

}

#endif // MODEL_MESH_H
