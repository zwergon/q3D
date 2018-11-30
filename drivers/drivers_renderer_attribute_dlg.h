#ifndef DRIVERS_RENDERER_ATTRIBUTE_DLG_H
#define DRIVERS_RENDERER_ATTRIBUTE_DLG_H

#include <q3D/drivers/drivers_global.h>

#include <QDialog>

#include <q3D/model/renderer_attribute_dialog.h>
namespace Ui {
class RendererAttributeDlg;
}

namespace Q3D {
class DriversRendererAttribute;

class DriversRendererAttributeDlg :
        public QDialog /*UI*/,
        public RendererAttributeDialog /*data interface*/
{
    Q_OBJECT
    
public:
    explicit DriversRendererAttributeDlg(
            DriversRendererAttribute* attribute, QWidget *parent = 0);
    ~DriversRendererAttributeDlg();

    DriversRendererAttribute* getDriversAttribute() const;
    virtual QDialog* getDialog();

private slots:
    virtual void on_face_cb__toggled( bool checked );
    virtual void on_grid_cb__toggled( bool checked );
    virtual void on_nodes_cb__toggled( bool checked );
    virtual void on_nodes_color_button__colorChanged();
    virtual void on_grid_color_button__colorChanged();
    virtual void on_face_color_button__colorChanged();
    
private:
    Ui::RendererAttributeDlg* ui;
};

}

#endif // DRIVERS_RENDERER_ATTRIBUTE_DLG_H
