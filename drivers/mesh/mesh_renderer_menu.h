#ifndef MESH_RENDERER_MENU_H
#define MESH_RENDERER_MENU_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/gui/renderer_menu.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT MeshRendererMenu : public RendererMenu
{
    Q_OBJECT
public:
    MeshRendererMenu();
protected:
    virtual void onEditAttributesAction();
};

}

#endif // MESH_RENDERER_MENU_H
