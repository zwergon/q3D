#include "itk_mesh_renderer_attribute.h"

#include <q3D/model/renderer.h>
#include <q3D/itk/itk_mesh_renderer_attribute_dlg.h>

namespace Q3D {

ItkMeshRendererAttribute::ItkMeshRendererAttribute(ModelRenderer *parent) :
    RendererAttribute(parent),
    with_nodes_( true ),
    with_grid_( true ),
    with_faces_( true ),
    node_color_(0,0,0),
    grid_color_(0,0,0),
    face_color_(200, 200, 0),
    opacity_(1.0)
{
}

RendererAttributeDialog* ItkMeshRendererAttribute::createDialog(QWidget* parent){
    return new ItkMeshRendererAttributeDlg(this, parent);
}


void ItkMeshRendererAttribute::setOpacity(float opacity){
    opacity_ = opacity;
    face_color_.setAlphaF( opacity_ );
    emit attributeChanged(this);
}


void ItkMeshRendererAttribute::setWithNode( bool with ){
    with_nodes_ = with;
    emit attributeChanged( this );
}


void ItkMeshRendererAttribute::setWithFaces( bool with ){
    with_faces_ = with;
    emit attributeChanged( this );
}


void ItkMeshRendererAttribute::setWithGrid( bool with ){
    with_grid_ = with;
    emit attributeChanged( this );
}

void ItkMeshRendererAttribute::setNodeColor( const QColor& color ){
    node_color_ = color;
    emit attributeChanged( this );
}


void ItkMeshRendererAttribute::setGridColor( const QColor& color ){
    grid_color_ = color;
    emit attributeChanged( this );
}


void ItkMeshRendererAttribute::setFaceColor( const QColor& color ){
    face_color_ = color;
    emit attributeChanged( this );
}

}
