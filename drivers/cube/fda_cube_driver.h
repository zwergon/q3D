#ifndef FDACUBEDRIVER_H
#define FDACUBEDRIVER_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/model_driver.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT FdaCubeDriver : public ModelDriver
{
public:
    FdaCubeDriver();

    virtual Model* open( const ModelOpenInfo& );
    virtual void save( const Model& model, const QString& filename );
};

}
#endif // FDACUBEDRIVER_H