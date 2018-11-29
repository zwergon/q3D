#include "renderer_menu.h"

#include <QDebug>

#include <q3D/model/colormap.h>
#include <q3D/gui/main_window.h>
#include <q3D/gui/colormap_dlg.h>
#include <q3D/gui/renderer_attribute_dlg.h>
#include <q3D/model/renderer.h>


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
    if ( 0 != menu ){
        menu->addAction( "Attributes", this, SLOT(onEditAttributesAction()));
        menu->addAction( "Edit Colormap", this, SLOT(onEditColorMapAction()));

    }

}



void
RendererMenu::onEditColorMapAction(){
    ColorMap* colormap = renderer_->colormap();
    if ( 0 != colormap ){
        ColorMapDlg* dlg = new ColorMapDlg( window_ );
        dlg->setOriginalColorMap( colormap );
        dlg->show();
    }
}

void
RendererMenu::onEditAttributesAction(){
    RendererAttribute* attr = renderer_->attribute();
    if ( 0 != attr ){
        RendererAttributeDlg* dlg = new RendererAttributeDlg( attr, window_ );
        dlg->show();
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

