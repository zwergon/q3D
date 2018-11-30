#include "drivers_renderer_attribute.h"

#include <q3D/model/renderer.h>
#include <q3D/drivers/drivers_renderer_attribute_dlg.h>

namespace Q3D {

DriversRendererAttribute::DriversRendererAttribute(ModelRenderer *parent) :
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

RendererAttributeDialog* DriversRendererAttribute::createDialog(QWidget* parent){
    return new DriversRendererAttributeDlg(this, parent);
}


void DriversRendererAttribute::setOpacity(float opacity){
    opacity_ = opacity;
    face_color_.setAlphaF( opacity_ );
    emit attributeChanged(this);
}


void DriversRendererAttribute::setWithNode( bool with ){
    with_nodes_ = with;
    emit attributeChanged( this );
}


void DriversRendererAttribute::setWithFaces( bool with ){
    with_faces_ = with;
    emit attributeChanged( this );
}


void DriversRendererAttribute::setWithGrid( bool with ){
    with_grid_ = with;
    emit attributeChanged( this );
}

void DriversRendererAttribute::setNodeColor( const QColor& color ){
    node_color_ = color;
    emit attributeChanged( this );
}


void DriversRendererAttribute::setGridColor( const QColor& color ){
    grid_color_ = color;
    emit attributeChanged( this );
}


void DriversRendererAttribute::setFaceColor( const QColor& color ){
    face_color_ = color;
    emit attributeChanged( this );
}

}
