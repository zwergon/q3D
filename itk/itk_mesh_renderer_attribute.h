#ifndef ITK_MESH_RENDERER_ATTRIBUTE_H
#define ITK_MESH_RENDERER_ATTRIBUTE_H

#include <q3D/itk/itk_global.h>

#include <QObject>
#include <QColor>

#include <q3D/model/renderer_attribute.h>

namespace Q3D {

class ITKSHARED_EXPORT ItkMeshRendererAttribute : public RendererAttribute
{
    Q_OBJECT

public:
    explicit ItkMeshRendererAttribute( ModelRenderer *parent = 0);

    virtual RendererAttributeDialog* createDialog( QWidget* parent = nullptr );

    void setWithNode( bool  );
    bool withNodes() const { return with_nodes_; }

    void setWithGrid( bool );
    bool withGrid() const { return with_grid_; }

    void setWithFaces( bool );
    bool withFaces() const { return with_faces_; }

    void setNodeColor( const QColor& color );
    const QColor& nodeColor() const { return node_color_; }

    void setGridColor( const QColor& color );
    const QColor& gridColor() const { return grid_color_; }

    void setFaceColor( const QColor& color );
    const QColor& faceColor() const { return face_color_; }

    void setOpacity( float opacity );
    float opacity() const { return opacity_; }

private:
    bool with_nodes_;
    bool with_grid_;
    bool with_faces_;
    QColor node_color_;
    QColor grid_color_;
    QColor face_color_;
    float opacity_;

};


}

#endif //ITK_MESH_RENDERER_ATTRIBUTE_H
