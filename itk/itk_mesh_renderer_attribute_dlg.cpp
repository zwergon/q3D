#include "itk_mesh_renderer_attribute_dlg.h"
#include "ui_itk_mesh_renderer_attribute_dlg.h"

#include <q3D/model/renderer_attribute.h>
#include <q3D/itk/itk_mesh_renderer_attribute.h>

namespace Q3D {

ItkMeshRendererAttributeDlg::ItkMeshRendererAttributeDlg(
        ItkMeshRendererAttribute* attribute,
        QWidget *parent ) :
    QDialog(parent),
    RendererAttributeDialog(attribute),
    ui(new Ui::ItkMeshRendererAttributeDlg)
{
    ui->setupUi(this);
    ui->face_cb_->setChecked( attribute->withFaces() );
    ui->nodes_cb_->setChecked( attribute->withNodes() );
    ui->grid_cb_->setChecked( attribute->withGrid() );
    ui->grid_color_button_->setColor( attribute->gridColor() );
    ui->nodes_color_button_->setColor( attribute->nodeColor() );
    ui->face_color_button_->setColor( attribute->faceColor() );
}

ItkMeshRendererAttributeDlg::~ItkMeshRendererAttributeDlg()
{
    delete ui;
}

ItkMeshRendererAttribute* ItkMeshRendererAttributeDlg::getDriversAttribute() const {
    return static_cast<ItkMeshRendererAttribute*>(getAttribute());
}

QDialog* ItkMeshRendererAttributeDlg::getDialog() {
    return this;
}

void ItkMeshRendererAttributeDlg::on_face_cb__toggled( bool checked ){
    getDriversAttribute()->setWithFaces( checked );
}

void ItkMeshRendererAttributeDlg::on_grid_cb__toggled( bool checked ){
    getDriversAttribute()->setWithGrid( checked );
}

void ItkMeshRendererAttributeDlg::on_nodes_cb__toggled( bool checked ){
    getDriversAttribute()->setWithNode( checked );
}

void ItkMeshRendererAttributeDlg::on_nodes_color_button__colorChanged(){
    getDriversAttribute()->setNodeColor( ui->nodes_color_button_->color() );
}

void ItkMeshRendererAttributeDlg::on_grid_color_button__colorChanged(){
    getDriversAttribute()->setGridColor( ui->grid_color_button_->color() );
}

void ItkMeshRendererAttributeDlg::on_face_color_button__colorChanged(){
    getDriversAttribute()->setFaceColor( ui->face_color_button_->color() );
}


}
