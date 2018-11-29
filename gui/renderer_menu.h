#ifndef RENDERER_MENU_H
#define RENDERER_MENU_H

#include <q3D/gui/gui_scope.h>
#include <q3D/model/factory.hpp>

#include <QMenu>

namespace Q3D {

class Model;
class CGlWindow;
class ModelRenderer;
class RendererMenu;

/**************************************************************/

class GUISHARED_EXPORT RendererMenuFactory : public ObjectFactory<RendererMenu, QString> {
public:
    RendererMenuFactory();
};

/**************************************************************/

class GUISHARED_EXPORT RendererMenu : public QObject {
    Q_OBJECT
public:
    static RendererMenuFactory& factory();
    static RendererMenu* create(
            QMenu* menu,
            CGlWindow* window,
            ModelRenderer* renderer );
public:
    RendererMenu();


signals:
    void requestDelete( Model* model );

protected:
    virtual void init( CGlWindow* window, ModelRenderer* renderer );

protected slots:
    virtual void onEditColorMapAction();
    virtual void onEditAttributesAction();
    virtual void onSaveAction();
    virtual void onDeleteAction();

protected:
    ModelRenderer* renderer_;
    CGlWindow* window_;
    static RendererMenuFactory menu_factory_;
};




}


#endif // RENDERER_MENU_H
