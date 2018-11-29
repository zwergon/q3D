#include "mesh_renderer_menu.h"

#include <QDebug>
#include "mesh_renderer_attribute.h"
#include "mesh_renderer_attribute_dlg.h"
#include <q3D/gui/main_window.h>
#include <q3D/model/renderer.h>


namespace Q3D {

MeshRendererMenu::MeshRendererMenu()
{
}

void MeshRendererMenu::onEditAttributesAction(){
    MeshRendererAttribute* attr = static_cast<MeshRendererAttribute*>( renderer_->attribute() );
    if ( 0 != attr ){
        QDialog* dlg = new MeshRendererAttributeDlg( attr, window_ );
        dlg->show();
    }
}

}
