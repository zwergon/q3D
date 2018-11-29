#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include <q3D/cube/cube_global.h>

#include <q3D/model/model.h>
#include <q3D/cube/cube.h>

namespace Q3D {

class CUBESHARED_EXPORT CubeModel : public Model
{
    Q_OBJECT

public:
    CubeModel();
    Cube& cube();

    virtual void update();

private:
    Cube  cube_;
};

inline Cube& CubeModel::cube() {
    return cube_;
}

}

#endif // CUBE_MODEL_H
