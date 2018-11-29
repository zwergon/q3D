#include "renderer_attribute.h"

#include <q3D/model/renderer.h>

namespace Q3D {

RendererAttribute::RendererAttribute(ModelRenderer *parent) :
    QObject(parent),
    with_nodes_( true ),
    with_grid_( true ),
    with_faces_( true ),
    node_color_(0,0,0),
    grid_color_(0,0,0),
    face_color_(200, 200, 0),
    opacity_(1.0)
{
}

ModelRenderer* RendererAttribute::renderer() const {
    return static_cast<ModelRenderer*>( parent() );
}

void RendererAttribute::setOpacity(float opacity){
    opacity_ = opacity;
    face_color_.setAlphaF( opacity_ );
    emit attributeChanged(this);
}


void RendererAttribute::setWithNode( bool with ){
    with_nodes_ = with;
    emit attributeChanged( this );
}


void RendererAttribute::setWithFaces( bool with ){
    with_faces_ = with;
    emit attributeChanged( this );
}


void RendererAttribute::setWithGrid( bool with ){
    with_grid_ = with;
    emit attributeChanged( this );
}

void RendererAttribute::setNodeColor( const QColor& color ){
    node_color_ = color;
    emit attributeChanged( this );
}


void RendererAttribute::setGridColor( const QColor& color ){
    grid_color_ = color;
    emit attributeChanged( this );
}


void RendererAttribute::setFaceColor( const QColor& color ){
    face_color_ = color;
    emit attributeChanged( this );
}

}
