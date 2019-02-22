#include "renderer_menu.h"

#include <QDebug>
#include <QDialog>
#include <QPluginLoader>

#include <q3D/model/model.h>
#include <q3D/model/colormap.h>
#include <q3D/model/renderer_attribute.h>
#include <q3D/model/renderer_attribute_dialog.h>
#include <q3D/model/renderer.h>

#include <q3D/plugins/plugins.h>
#include <q3D/plugins/plugin_action.h>
#include <q3D/plugins/plugin_collection.h>

#include <q3D/gui/main_window.h>
#include <q3D/gui/colormap_dlg.h>

namespace Q3D {

RendererMenuFactory::RendererMenuFactory(){
    registerFactory<RendererMenu>( ModelRenderer::staticMetaObject.className() );
}

RendererMenuFactory RendererMenu::menu_factory_;

RendererMenuFactory& RendererMenu::factory(){
    return menu_factory_;
}

RendererMenu* RendererMenu::create( QMenu* menu, CGlWindow *window, ModelRenderer *renderer){

    RendererMenu* renderer_menu = 0;
    const QMetaObject* meta_object = renderer->metaObject();
    while( meta_object != 0 ){
        renderer_menu = menu_factory_.create( meta_object->className() );
        if ( 0 != renderer_menu ){
            break;
        }
        meta_object = meta_object->superClass();

    }
    if ( 0 != renderer_menu ){
        renderer_menu->setParent( menu );
        renderer_menu->init( window, renderer );
    }
    return renderer_menu;
}

RendererMenu::RendererMenu()
    : QObject(),
      renderer_(0)
{
}

void RendererMenu::init(CGlWindow* window, ModelRenderer* renderer){

    renderer_ = renderer;
    window_ = window;

    QMenu* menu = qobject_cast<QMenu*>( parent() );
    if ( nullptr != menu ){
        menu->addAction( "Attributes", this, &RendererMenu::onEditAttributesAction);
        menu->addAction( "Edit Colormap", this, &RendererMenu::onEditColorMapAction);
    }

    Model* model = renderer_->model();

    QStringList fileNames = Plugins::instance()->get_plugins();
    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);
        QObject *plugin = loader.instance();
        PluginCollection* plugin_collection =  qobject_cast<PluginCollection*>(plugin);
        if (plugin_collection != nullptr) {
            ActionInterface* action_interface = plugin_collection->getActionPlugin();
            if ( nullptr != action_interface ){
                QMenu* submenu = new QMenu(action_interface->name(), menu);
                bool is_empty = true;
                foreach( auto action, action_interface->getActions(submenu)){
                    if ( ( action->getType() == PluginAction::MENU_ACTION ) &&
                         action->canWorkOn(model) ){
                        is_empty = false;
                        submenu->addAction(action->getAction());
                        connect( action,  &PluginAction::activated, this, &RendererMenu::onActionActivated );
                    }
                }

                if ( is_empty ){
                    delete submenu;
                }
                else {
                    menu->addMenu(submenu);
                }
            }
        }
    }



    model->popupMenu(menu);

}

void RendererMenu::onActionActivated(PluginAction* plugin_action){
    if ( nullptr != renderer_ ){
        plugin_action->execute(renderer_->model());
    }
}



void
RendererMenu::onEditColorMapAction(){
    ColorMap* colormap = renderer_->colormap();
    if ( nullptr != colormap ){
        ColorMapDlg* dlg = new ColorMapDlg( window_ );
        dlg->setOriginalColorMap( colormap );
        dlg->show();
    }
}

void
RendererMenu::onEditAttributesAction(){
    RendererAttribute* attr = renderer_->attribute();
    if ( nullptr != attr ){
        RendererAttributeDialog* dlg = attr->createDialog( window_ );
        if ( nullptr != dlg->getDialog() ){
           dlg->getDialog()->show();
        }
    }
}

void
RendererMenu::onDeleteAction(){
    if ( 0 != renderer_ )
        emit requestDelete( renderer_->model() );
}


void
RendererMenu::onSaveAction(){
    //do nothing by default
}

}

