#include "mesh_renderer_attribute_dlg.h"
#include "ui_mesh_renderer_attribute_dlg.h"

#include <q3D/drivers/mesh/mesh_renderer_attribute.h>
#include "mesh_renderer.h"
#include "mesh_model.h"

#include <q3D/mesh/mesh.h>

Q_DECLARE_METATYPE( Q3D::PropertyInfo )

namespace Q3D {

MeshRendererAttributeDlg::MeshRendererAttributeDlg(MeshRendererAttribute* attribute, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshRendererAttributeDlg),
    attribute_( attribute )
{
    ui->setupUi(this);
    ui->face_cb_->setChecked( attribute->withFaces() );
    ui->nodes_cb_->setChecked( attribute->withNodes() );
    ui->grid_cb_->setChecked( attribute->withGrid() );
    ui->painted_cb_->setChecked( attribute->painted() );
    ui->grid_color_button_->setColor( attribute->gridColor() );
    ui->nodes_color_button_->setColor( attribute->nodeColor() );
    ui->face_color_button_->setColor( attribute->faceColor() );


    MeshRenderer* mesh_renderer = static_cast<MeshRenderer*>(attribute->renderer());
    MeshModel* mesh_model = static_cast<MeshModel*>(mesh_renderer->model());

    Mesh& mesh = mesh_model->mesh();
    QList<Property> properties;
    mesh.get_all_properties( properties );
    QListIterator<Property> itp( properties );
    while (itp.hasNext() ){
        const Property& prop = itp.next();
        PropertyInfo pinfo( prop.name(), prop.size(), prop.type() );
        ui->painted_combo_->addItem( prop.name(), qVariantFromValue(pinfo) );
    }
}

MeshRendererAttributeDlg::~MeshRendererAttributeDlg()
{
    delete ui;
}

void MeshRendererAttributeDlg::on_face_cb__toggled( bool checked ){
    attribute_->setWithFaces( checked );
}

void MeshRendererAttributeDlg::on_painted_combo__currentIndexChanged ( int ){
    if ( ui->painted_cb_->isChecked() ){
        selectProperty();
    }
}

void MeshRendererAttributeDlg::selectProperty(){
    PropertyInfo pinfo = ui->painted_combo_->itemData( ui->painted_combo_->currentIndex() ).value<PropertyInfo>();

    MeshRenderer* mesh_renderer = static_cast<MeshRenderer*>(attribute_->renderer());
    MeshModel* mesh_model = static_cast<MeshModel*>(mesh_renderer->model());
    Mesh& mesh = mesh_model->mesh();

    Property prop = mesh.find_property( pinfo, false );
    if ( prop != Property::null ){
        attribute_->setSelectedProperty( prop );
    }
}

void MeshRendererAttributeDlg::on_painted_cb__toggled( bool checked ){
     if ( checked ) {
         attribute_->blockSignals(true);
         selectProperty();
         attribute_->blockSignals(false);
     }
    attribute_->setPainted( checked );

}

void MeshRendererAttributeDlg::on_grid_cb__toggled( bool checked ){
    attribute_->setWithGrid( checked );
}

void MeshRendererAttributeDlg::on_nodes_cb__toggled( bool checked ){
    attribute_->setWithNode( checked );
}

void MeshRendererAttributeDlg::on_nodes_color_button__colorChanged(){
    attribute_->setNodeColor( ui->nodes_color_button_->color() );
}

void MeshRendererAttributeDlg::on_grid_color_button__colorChanged(){
    attribute_->setGridColor( ui->grid_color_button_->color() );
}

void MeshRendererAttributeDlg::on_face_color_button__colorChanged(){
    attribute_->setFaceColor( ui->face_color_button_->color() );
}

void MeshRendererAttributeDlg::on_transparency_slider__valueChanged( int value ){
    if ( ui->transparency_group_box_->isChecked() ){
        attribute_->setOpacity( (float)value/100. );
    }
}

void MeshRendererAttributeDlg::on_transparency_group_box__toggled( bool checked ){
    if ( checked ){
        float value = (float)ui->transparency_slider_->value()/100.;
        attribute_->setOpacity( value );
    }
    else {
        attribute_->setOpacity( 1.0 );
    }
}
}
