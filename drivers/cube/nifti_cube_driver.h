#ifndef NIFTI_CUBE_DRIVER_H
#define NIFTI_CUBE_DRIVER_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/model_driver.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT NiftiCubeDriver : public ModelDriver
{
public:
    NiftiCubeDriver();

    virtual Model* open( const ModelOpenInfo& );
    virtual void save( const Model& model, const QString& filename );
};

}
#endif // NIFTI_CUBE_DRIVER_H
