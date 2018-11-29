#ifndef RENDERER_ATTRIBUTE_DLG_H
#define RENDERER_ATTRIBUTE_DLG_H

#include <QDialog>

namespace Ui {
class RendererAttributeDlg;
}

namespace Q3D {
class RendererAttribute;


class RendererAttributeDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit RendererAttributeDlg( RendererAttribute* attribute, QWidget *parent = 0);
    ~RendererAttributeDlg();

private slots:
    virtual void on_face_cb__toggled( bool checked );
    virtual void on_grid_cb__toggled( bool checked );
    virtual void on_nodes_cb__toggled( bool checked );
    virtual void on_nodes_color_button__colorChanged();
    virtual void on_grid_color_button__colorChanged();
    virtual void on_face_color_button__colorChanged();
    
private:
    Ui::RendererAttributeDlg *ui;
    RendererAttribute* attribute_;
};

}

#endif // RENDERER_ATTRIBUTE_DLG_H
