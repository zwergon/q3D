#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include <q3D/drivers/drivers_global.h>

#include <QMenu>

#include <q3D/model/model.h>
#include <q3D/cube/cube.h>


namespace Q3D {

class PluginAction;

class DRIVERSSHARED_EXPORT CubeModel : public Model
{
    Q_OBJECT

public:
    CubeModel();
    virtual ~CubeModel();

    Cube& cube();
    const Cube& cube() const;
    void setCube( Cube* cube );

    virtual void update() override;
    virtual QString tooltip() const override;

private:
    Cube*  cube_;
};

inline Cube& CubeModel::cube() {
    return *cube_;
}

inline const Cube& CubeModel::cube() const  {
    return *cube_;
}

inline void CubeModel::setCube( Cube* cube ) {
    cube_ = cube;
}

}

#endif // CUBE_MODEL_H
