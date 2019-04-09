#ifndef ITK_MESH_RENDERER_ATTRIBUTE_DLG_H
#define ITK_MESH_RENDERER_ATTRIBUTE_DLG_H

#include <q3D/itk/itk_global.h>

#include <QDialog>

#include <q3D/model/renderer_attribute_dialog.h>

namespace Ui {
class ItkMeshRendererAttributeDlg;
}

namespace Q3D {
class ItkMeshRendererAttribute;

class ItkMeshRendererAttributeDlg :
        public QDialog /*UI*/,
        public RendererAttributeDialog /*data interface*/
{
    Q_OBJECT
    
public:
    explicit ItkMeshRendererAttributeDlg(
            ItkMeshRendererAttribute* attribute, QWidget *parent = 0);
    ~ItkMeshRendererAttributeDlg();

    ItkMeshRendererAttribute* getDriversAttribute() const;
    virtual QDialog* getDialog();

private slots:
    virtual void on_face_cb__toggled( bool checked );
    virtual void on_grid_cb__toggled( bool checked );
    virtual void on_nodes_cb__toggled( bool checked );
    virtual void on_nodes_color_button__colorChanged();
    virtual void on_grid_color_button__colorChanged();
    virtual void on_face_color_button__colorChanged();
    
private:
    Ui::ItkMeshRendererAttributeDlg* ui;
};

}

#endif // ITK_MESH_RENDERER_ATTRIBUTE_DLG_H
