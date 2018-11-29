#ifndef RENDERER_ATTRIBUTE_H
#define RENDERER_ATTRIBUTE_H

#include <q3D/model/model_scope.h>

#include <QObject>
#include <QColor>

#include <q3D/model/model_scope.h>

namespace Q3D {

class ModelRenderer;

class MODELSHARED_EXPORT RendererAttribute : public QObject
{
    Q_OBJECT

public:
    explicit RendererAttribute( ModelRenderer *parent = 0);

    ModelRenderer* renderer() const;

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

    
signals:
    void attributeChanged( RendererAttribute* );
    
public slots:

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

#endif // RENDERER_ATTRIBUTE_H
