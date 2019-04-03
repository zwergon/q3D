#include "cube_renderer_attribute_dlg.h"
#include "ui_cube_renderer_attribute_dlg.h"

#include <q3D/cube/cube.h>

#include <q3D/drivers/cube/cube_renderer_attribute.h>
#include <q3D/drivers/cube/cube_renderer.h>
#include <q3D/drivers/cube/cube_model.h>

namespace Q3D {

CubeRendererAttributeDlg::CubeRendererAttributeDlg(
        CubeRendererAttribute* renderer_attribute,
        QWidget *parent) :
    QDialog(parent),
    RendererAttributeDialog(renderer_attribute),
    ui(new Ui::CubeRendererAttributeDlg)
{
    ui->setupUi(this);

    CubeModel* cube_model =
            static_cast<CubeModel*>(renderer_attribute->renderer()->model());

    Cube& cube = cube_model->cube();

    ui->dial_x_->setRange(0, cube.nx()-1);
    ui->dial_x_->setValue( renderer_attribute->getCursorX() );
    ui->x_edit_->setText( QString::number(renderer_attribute->getCursorX()));

    ui->dial_y_->setRange(0, cube.ny()-1);
    ui->dial_y_->setValue( renderer_attribute->getCursorY() );
    ui->y_edit_->setText( QString::number(renderer_attribute->getCursorY()));

    ui->dial_z_->setRange(0, cube.nz()-1);
    ui->dial_z_->setValue( renderer_attribute->getCursorZ() );
    ui->z_edit_->setText( QString::number(renderer_attribute->getCursorZ()));

    ui->transparency_slider_->setValue( (1. - renderer_attribute->opacity()) * 100. );
}

CubeRendererAttributeDlg::~CubeRendererAttributeDlg()
{
    delete ui;
}

CubeRendererAttribute* CubeRendererAttributeDlg::getCubeAttribute() const {
    return static_cast<CubeRendererAttribute*>(getAttribute());
}

QDialog* CubeRendererAttributeDlg::getDialog() {
    return this;
}

void CubeRendererAttributeDlg::on_transparency_slider__valueChanged(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());
    float opacity = (100. - value)*.01;
    cube_attribute->setOpacity(opacity);
}

void CubeRendererAttributeDlg::on_dial_x__valueChanged(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorX(value);
    ui->x_edit_->blockSignals(true);
    ui->x_edit_->setText( QString::number(value));
    ui->x_edit_->blockSignals(false);
}

void CubeRendererAttributeDlg::on_dial_y__valueChanged(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorY(value);
    ui->y_edit_->blockSignals(true);
    ui->y_edit_->setText( QString::number(value));
    ui->y_edit_->blockSignals(false);
}

void CubeRendererAttributeDlg::on_dial_z__valueChanged(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorZ(value);
    ui->z_edit_->blockSignals(true);
    ui->z_edit_->setText( QString::number(value));
    ui->z_edit_->blockSignals(false);
}

void CubeRendererAttributeDlg::on_x_edit__textChanged(){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    int value = ui->x_edit_->text().toInt();
    cube_attribute->setCursorX(value);
    ui->dial_x_->blockSignals(true);
    ui->dial_x_->setValue(value);
    ui->dial_x_->blockSignals(false);
}

void CubeRendererAttributeDlg::on_y_edit__textChanged(){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    int value = ui->y_edit_->text().toInt();
    cube_attribute->setCursorY(value);
    ui->dial_y_->blockSignals(true);
    ui->dial_y_->setValue(value);
    ui->dial_y_->blockSignals(false);
}

void CubeRendererAttributeDlg::on_z_edit__textChanged(){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    int value = ui->z_edit_->text().toInt();
    cube_attribute->setCursorZ(value);
    ui->dial_z_->blockSignals(true);
    ui->dial_z_->setValue(value);
    ui->dial_z_->blockSignals(false);
}

}
