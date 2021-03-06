#ifndef RENDERER_ATTRIBUTE_DLG_H
#define RENDERER_ATTRIBUTE_DLG_H

#include <q3D/drivers/drivers_global.h>

#include <QDialog>

#include <q3D/model/renderer_attribute_dialog.h>

namespace Ui {
class MeshRendererAttributeDlg;
}

namespace Q3D {
class MeshRendererAttribute;

class DRIVERSSHARED_EXPORT MeshRendererAttributeDlg :
        public QDialog /*UI*/,
        public RendererAttributeDialog /*data interface*/
{
    Q_OBJECT
    
public:
    explicit MeshRendererAttributeDlg( MeshRendererAttribute* attribute, QWidget *parent = 0);
    ~MeshRendererAttributeDlg();

    MeshRendererAttribute* getMeshAttribute() const;
    virtual QDialog* getDialog();

private slots:
    virtual void on_face_cb__toggled( bool checked );
    virtual void on_grid_cb__toggled( bool checked );
    virtual void on_nodes_cb__toggled( bool checked );
    virtual void on_painted_cb__toggled( bool checked );
    virtual void on_nodes_color_button__colorChanged();
    virtual void on_grid_color_button__colorChanged();
    virtual void on_face_color_button__colorChanged();
    virtual void on_painted_combo__currentIndexChanged ( int index );
    virtual void on_transparency_slider__valueChanged( int );
    virtual void on_transparency_group_box__toggled( bool checked );

private:
    void selectProperty();
    
private:
    Ui::MeshRendererAttributeDlg *ui;
};

}

#endif // RENDERER_ATTRIBUTE_DLG_H
