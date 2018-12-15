#ifndef CUBE_RENDERER_ATTRIBUTE_DLG_H
#define CUBE_RENDERER_ATTRIBUTE_DLG_H

#include <q3D/cube/cube_global.h>

#include <QDialog>

#include <q3D/model/renderer_attribute_dialog.h>

namespace Ui {
class CubeRendererAttributeDlg;
}

namespace Q3D {

class CubeRendererAttribute;

class CubeRendererAttributeDlg :
        public QDialog,
        public RendererAttributeDialog
{
    Q_OBJECT

public:
    explicit CubeRendererAttributeDlg(
            CubeRendererAttribute* renderer_attribute,
            QWidget *parent = 0);
    ~CubeRendererAttributeDlg();

    CubeRendererAttribute* getCubeAttribute() const;
    virtual QDialog* getDialog();

private slots:
    void on_dial_x__valueChanged(int);
    void on_dial_y__valueChanged(int);
    void on_dial_z__valueChanged(int);

private:
    Ui::CubeRendererAttributeDlg *ui;
};

}

#endif // CUBE_RENDERER_ATTRIBUTE_DLG_H
