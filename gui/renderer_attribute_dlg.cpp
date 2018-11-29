#include "renderer_attribute_dlg.h"
#include "ui_renderer_attribute_dlg.h"

#include <q3D/model/renderer_attribute.h>

namespace Q3D {

RendererAttributeDlg::RendererAttributeDlg(RendererAttribute* attribute, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RendererAttributeDlg),
    attribute_( attribute )
{
    ui->setupUi(this);
    ui->face_cb_->setChecked( attribute->withFaces() );
    ui->nodes_cb_->setChecked( attribute->withNodes() );
    ui->grid_cb_->setChecked( attribute->withGrid() );
    ui->grid_color_button_->setColor( attribute->gridColor() );
    ui->nodes_color_button_->setColor( attribute->nodeColor() );
    ui->face_color_button_->setColor( attribute->faceColor() );
}

RendererAttributeDlg::~RendererAttributeDlg()
{
    delete ui;
}

void RendererAttributeDlg::on_face_cb__toggled( bool checked ){
    attribute_->setWithFaces( checked );
}

void RendererAttributeDlg::on_grid_cb__toggled( bool checked ){
    attribute_->setWithGrid( checked );
}

void RendererAttributeDlg::on_nodes_cb__toggled( bool checked ){
    attribute_->setWithNode( checked );
}

void RendererAttributeDlg::on_nodes_color_button__colorChanged(){
    attribute_->setNodeColor( ui->nodes_color_button_->color() );
}

void RendererAttributeDlg::on_grid_color_button__colorChanged(){
    attribute_->setGridColor( ui->grid_color_button_->color() );
}

void RendererAttributeDlg::on_face_color_button__colorChanged(){
    attribute_->setFaceColor( ui->face_color_button_->color() );
}


}
