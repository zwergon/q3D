#ifndef ITK_MESH_DRIVER_H
#define ITK_MESH_DRIVER_H

#include <q3D/itk/itk_global.h>

#include <q3D/model/model_driver.h>

namespace Q3D {

class ITKSHARED_EXPORT ItkMeshDriver : public ModelDriver
{
    Q_OBJECT
public:
    ItkMeshDriver();

    virtual ModelOpenInfo* openInfo() const override;
    virtual bool canHandle(Model *) const override;
    virtual Q3D::Model*  open( const ModelOpenInfo& ) override;
    virtual void         save( const Model& model, const ModelOpenInfo&) override;
};

}

#endif // ITK_MESH_DRIVER_H
