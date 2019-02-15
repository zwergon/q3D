#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include <q3D/cube/cube_global.h>

#include <QMenu>

#include <q3D/model/model.h>
#include <q3D/cube/cube.h>


namespace Q3D {

class CUBESHARED_EXPORT CubeModel : public Model
{
    Q_OBJECT

public:
    CubeModel();
    virtual ~CubeModel();

    Cube& cube();
    const Cube& cube() const;
    void setCube( Cube* cube );

    virtual void update();

    virtual void popupMenu(QMenu* );

private slots:
    virtual void onCubeLaunchAction();

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
