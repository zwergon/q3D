#ifndef ITK_MESH_RENDERER_H
#define ITK_MESH_RENDERER_H

#include <q3D/itk/itk_global.h>

#include <q3D/model/renderer.h>

namespace Q3D {

class ItkMeshModel;

class ITKSHARED_EXPORT ItkMeshRenderer : public ModelRenderer
{
    Q_OBJECT
public:
    ItkMeshRenderer();

    virtual void update(RendererArea *) override;
    virtual RendererAttribute* createAttribute() override;

protected:
    void drawEdges( ItkMeshModel* mesh_model ) const;
    void drawSurface( ItkMeshModel* mesh_model ) const;
};

}

#endif // ITK_MESH_RENDERER_H
