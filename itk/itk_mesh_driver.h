#ifndef ITK_MESH_DRIVER_H
#define ITK_MESH_DRIVER_H

#include <q3D/itk/itk_global.h>

#include <q3D/model/model_driver.h>

class ITKSHARED_EXPORT ItkMeshDriver : public Q3D::ModelDriver
{
    Q_OBJECT
public:
    ItkMeshDriver();
};

#endif // ITK_MESH_DRIVER_H
