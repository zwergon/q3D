#include "mesh_renderer_attribute.h"

#include <q3D/model/renderer.h>
#include <q3D/model/colormap.h>

#include <q3D/mesh/property.h>
#include <q3D/mesh/property_db.h>
#include <q3D/mesh/property_cube.h>

#include "mesh_model.h"

#include <q3D/drivers/mesh/mesh_renderer_attribute_dlg.h>


namespace Q3D {

MeshRendererAttribute::MeshRendererAttribute( ModelRenderer* parent )
    : DriversRendererAttribute(parent),
      painted_(false),
      selected_property_(Property::null)
{
}

RendererAttributeDialog* MeshRendererAttribute::createDialog(QWidget *parent){
    return new MeshRendererAttributeDlg(this, parent);
}

void MeshRendererAttribute::setPainted( bool painted ) {
    painted_ = painted;
    emit attributeChanged(this);
}


void MeshRendererAttribute::setSelectedProperty(const Property &property ){

    selected_property_ = property;

    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::lowest();
    QListIterator<qint64> it_id( selected_property_.property_db()->cube().get_all() );
    while( it_id.hasNext() ){
        qint64 id = it_id.next();
        QVector<double> value;
        selected_property_.get_value( id, value );

        if ( value[0] != std::numeric_limits<double>::infinity() ){
            min = qMin( min, value[0] );
            max = qMax( max, value[0]);
        }
    }

    renderer()->colormap()->setMinMax( (float)min, (float)max );

    emit attributeChanged(this);
}

}
