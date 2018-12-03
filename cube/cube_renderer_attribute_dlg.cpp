#include "cube_renderer_attribute_dlg.h"
#include "ui_cube_renderer_attribute_dlg.h"

#include <q3D/cube/cube_renderer_attribute.h>
#include <q3D/cube/cube_renderer.h>
#include <q3D/cube/cube_model.h>
#include <q3D/cube/cube.h>

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

    ui->dial_x_->setRange(0, cube.getNx()-1);
    ui->dial_x_->setValue( renderer_attribute->getCursorX() );
    connect(ui->dial_x_, &QDial::valueChanged, this, &CubeRendererAttributeDlg::on_dial_x_value_changed);

    ui->dial_y_->setRange(0, cube.getNy()-1);
    ui->dial_y_->setValue( renderer_attribute->getCursorY() );
    connect(ui->dial_y_, &QDial::valueChanged, this, &CubeRendererAttributeDlg::on_dial_y_value_changed);


    ui->dial_z_->setRange(0, cube.getNz()-1);
    ui->dial_z_->setValue( renderer_attribute->getCursorZ() );
    connect(ui->dial_z_, &QDial::valueChanged, this, &CubeRendererAttributeDlg::on_dial_z_value_changed);

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

void CubeRendererAttributeDlg::on_dial_x_value_changed(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorX(value);
}

void CubeRendererAttributeDlg::on_dial_y_value_changed(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorY(value);
}

void CubeRendererAttributeDlg::on_dial_z_value_changed(int value){
    CubeRendererAttribute* cube_attribute =
            static_cast<CubeRendererAttribute*>(getAttribute());

    cube_attribute->setCursorZ(value);
}

}
