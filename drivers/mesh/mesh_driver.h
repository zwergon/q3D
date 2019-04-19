#ifndef MESH_DRIVER_H
#define MESH_DRIVER_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/model_driver.h>


namespace Q3D {


class DRIVERSSHARED_EXPORT MeshDriver : public ModelDriver
{
public:
    MeshDriver();

    virtual ModelOpenInfo* openInfo() const override;
    virtual bool canHandle(Model *) const override;
    virtual Model* open( const ModelOpenInfo& ) override;
    virtual void save( const Model& model, const ModelOpenInfo& ) override;
};

}
#endif // MESH_DRIVER_H
