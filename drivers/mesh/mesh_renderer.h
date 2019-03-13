#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include <q3D/drivers/drivers_global.h>

#include <QColor>

#include <q3D/model/renderer.h>
#include <q3D/mesh/property.h>

namespace Q3D {

class MeshModel;

class DRIVERSSHARED_EXPORT MeshRenderer : public ModelRenderer
{
    Q_OBJECT
public:
    MeshRenderer();

    virtual void init();
    virtual void update();
    virtual RendererAttribute* createAttribute();

private:
    void drawNodes( MeshModel* mesh );
    void drawEdges( MeshModel* mesh );
    void drawSurface( MeshModel* mesh );
    void drawPaintedCellSurface( MeshModel* mesh );
    void drawPaintedNodeSurface( MeshModel* mesh );

    void setColor( const Property& prop, quint64 id, const QColor& default_color = Qt::red );
};

}
#endif // MESH_RENDERER_H
