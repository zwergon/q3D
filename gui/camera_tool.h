#ifndef CAMERA_TOOL_H
#define CAMERA_TOOL_H

#include <q3D/gui/gui_global.h>

#include <q3D/gui/abstract_tool.h>
#include <q3D/gui/view_control.h>

namespace Q3D {

class GUISHARED_EXPORT CameraTool : public AbstractTool
{
public:
    CameraTool();

    virtual void  handleMousePressEvent  ( QMouseEvent * );
    virtual void  handleMouseMoveEvent   ( QMouseEvent * );
    virtual void  handleMouseReleaseEvent( QMouseEvent * );
    virtual void  handleWheelEvent       ( QWheelEvent * );


private:
    void cameraRotate( int angle_x, int angle_y );
    void cameraZoom( bool increase );
    void cameraTranslate( int translate_y, int translate_x );

private:
    int         x_prec_, y_prec_ ;
    bool        move_activated_;



};

}

#endif // CAMERA_TOOL_H
