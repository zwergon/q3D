#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <q3D/gui/gui_global.h>

#include <q3D/gui/gl_area.h>

namespace Q3D {

class GUISHARED_EXPORT AbstractTool
{
public:
    virtual ~AbstractTool();

    void setGlArea( CGlArea* area );
    CGlArea* getGlArea() const;

    bool isActive();
    virtual void activate();
    virtual void deactivate();

    virtual void  handleMousePressEvent  ( QMouseEvent * ) = 0;
    virtual void  handleMouseMoveEvent   ( QMouseEvent * ) = 0;
    virtual void  handleMouseReleaseEvent( QMouseEvent * ) = 0;
    virtual void  handleWheelEvent       ( QWheelEvent * ) = 0;

protected:
    AbstractTool();


protected:
    CGlArea* gl_area_;
};

inline void AbstractTool::setGlArea(CGlArea *area){
    gl_area_ = area;
}

inline CGlArea* AbstractTool::getGlArea() const {
    return gl_area_;
}

}

#endif // ABSTRACT_TOOL_H
