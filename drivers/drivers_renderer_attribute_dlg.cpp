#include "drivers_renderer_attribute_dlg.h"
#include "ui_renderer_attribute_dlg.h"

#include <q3D/model/renderer_attribute.h>
#include <q3D/drivers/drivers_renderer_attribute.h>

namespace Q3D {

DriversRendererAttributeDlg::DriversRendererAttributeDlg(
        DriversRendererAttribute* attribute,
        QWidget *parent ) :
    QDialog(parent),
    RendererAttributeDialog(attribute),
    ui(new Ui::RendererAttributeDlg)
{
    ui->setupUi(this);
    ui->face_cb_->setChecked( attribute->withFaces() );
    ui->nodes_cb_->setChecked( attribute->withNodes() );
    ui->grid_cb_->setChecked( attribute->withGrid() );
    ui->grid_color_button_->setColor( attribute->gridColor() );
    ui->nodes_color_button_->setColor( attribute->nodeColor() );
    ui->face_color_button_->setColor( attribute->faceColor() );
}

DriversRendererAttributeDlg::~DriversRendererAttributeDlg()
{
    delete ui;
}

DriversRendererAttribute* DriversRendererAttributeDlg::getDriversAttribute() const {
    return static_cast<DriversRendererAttribute*>(getAttribute());
}

QDialog* DriversRendererAttributeDlg::getDialog() {
    return this;
}

void DriversRendererAttributeDlg::on_face_cb__toggled( bool checked ){
    getDriversAttribute()->setWithFaces( checked );
}

void DriversRendererAttributeDlg::on_grid_cb__toggled( bool checked ){
    getDriversAttribute()->setWithGrid( checked );
}

void DriversRendererAttributeDlg::on_nodes_cb__toggled( bool checked ){
    getDriversAttribute()->setWithNode( checked );
}

void DriversRendererAttributeDlg::on_nodes_color_button__colorChanged(){
    getDriversAttribute()->setNodeColor( ui->nodes_color_button_->color() );
}

void DriversRendererAttributeDlg::on_grid_color_button__colorChanged(){
    getDriversAttribute()->setGridColor( ui->grid_color_button_->color() );
}

void DriversRendererAttributeDlg::on_face_color_button__colorChanged(){
    getDriversAttribute()->setFaceColor( ui->face_color_button_->color() );
}


}
