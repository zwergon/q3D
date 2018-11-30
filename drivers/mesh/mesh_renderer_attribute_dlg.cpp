#include "mesh_renderer_attribute_dlg.h"
#include "ui_mesh_renderer_attribute_dlg.h"

#include <q3D/drivers/mesh/mesh_renderer_attribute.h>
#include "mesh_renderer.h"
#include "mesh_model.h"

#include <q3D/mesh/mesh.h>
#include <q3D/model/renderer_attribute_dialog.h>

Q_DECLARE_METATYPE( Q3D::PropertyInfo )

namespace Q3D {

MeshRendererAttributeDlg::MeshRendererAttributeDlg(
        MeshRendererAttribute* attribute,
        QWidget *parent) :
    QDialog(parent),
    RendererAttributeDialog(attribute),
    ui(new Ui::MeshRendererAttributeDlg)
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

MeshRendererAttribute* MeshRendererAttributeDlg::getMeshAttribute() const {
    return static_cast<MeshRendererAttribute*>(getAttribute());
}

QDialog* MeshRendererAttributeDlg::getDialog() {
    return this;
}

MeshRendererAttributeDlg::~MeshRendererAttributeDlg()
{
    delete ui;
}

void MeshRendererAttributeDlg::on_face_cb__toggled( bool checked ){
    getMeshAttribute()->setWithFaces( checked );
}

void MeshRendererAttributeDlg::on_painted_combo__currentIndexChanged ( int ){
    if ( ui->painted_cb_->isChecked() ){
        selectProperty();
    }
}

void MeshRendererAttributeDlg::selectProperty(){
    PropertyInfo pinfo = ui->painted_combo_->itemData( ui->painted_combo_->currentIndex() ).value<PropertyInfo>();

    MeshRenderer* mesh_renderer = static_cast<MeshRenderer*>(getMeshAttribute()->renderer());
    MeshModel* mesh_model = static_cast<MeshModel*>(mesh_renderer->model());
    Mesh& mesh = mesh_model->mesh();

    Property prop = mesh.find_property( pinfo, false );
    if ( prop != Property::null ){
        getMeshAttribute()->setSelectedProperty( prop );
    }
}

void MeshRendererAttributeDlg::on_painted_cb__toggled( bool checked ){

    MeshRendererAttribute* attribute = getMeshAttribute();
     if ( checked ) {
         attribute->blockSignals(true);
         selectProperty();
         attribute->blockSignals(false);
     }
    attribute->setPainted( checked );

}

void MeshRendererAttributeDlg::on_grid_cb__toggled( bool checked ){
    getMeshAttribute()->setWithGrid( checked );
}

void MeshRendererAttributeDlg::on_nodes_cb__toggled( bool checked ){
    getMeshAttribute()->setWithNode( checked );
}

void MeshRendererAttributeDlg::on_nodes_color_button__colorChanged(){
    getMeshAttribute()->setNodeColor( ui->nodes_color_button_->color() );
}

void MeshRendererAttributeDlg::on_grid_color_button__colorChanged(){
    getMeshAttribute()->setGridColor( ui->grid_color_button_->color() );
}

void MeshRendererAttributeDlg::on_face_color_button__colorChanged(){
    getMeshAttribute()->setFaceColor( ui->face_color_button_->color() );
}

void MeshRendererAttributeDlg::on_transparency_slider__valueChanged( int value ){
    if ( ui->transparency_group_box_->isChecked() ){
        getMeshAttribute()->setOpacity( (float)value/100. );
    }
}

void MeshRendererAttributeDlg::on_transparency_group_box__toggled( bool checked ){
    if ( checked ){
        float value = (float)ui->transparency_slider_->value()/100.;
        getMeshAttribute()->setOpacity( value );
    }
    else {
        getMeshAttribute()->setOpacity( 1.0 );
    }
}
}
