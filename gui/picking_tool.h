#ifndef PICKING_TOOL_H
#define PICKING_TOOL_H


#include <q3D/gui/gui_global.h>

#include <q3D/gui/abstract_tool.h>
#include <q3D/gui/view_control.h>

namespace Q3D {

class GUISHARED_EXPORT PickingTool : public AbstractTool
{
public:
    PickingTool();

    virtual void activate() override;
    virtual void deactivate() override;

    virtual void  handleMousePressEvent  ( QMouseEvent * ) override;
    virtual void  handleMouseMoveEvent   ( QMouseEvent * ) override;
    virtual void  handleMouseReleaseEvent( QMouseEvent * ) override;
    virtual void  handleWheelEvent       ( QWheelEvent * ) override;

};

}

#endif // PICKING_TOOL_H
