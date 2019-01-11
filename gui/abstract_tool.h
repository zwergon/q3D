#ifndef ABSTRACT_TOOL_H
#define ABSTRACT_TOOL_H

#include <q3D/gui/gui_global.h>

#include <q3D/gui/gl_area.h>
#include <q3D/gui/tool_manager.h>
#include <QObject>

namespace Q3D {


class GUISHARED_EXPORT AbstractTool : public QObject
{
public:
    virtual ~AbstractTool();

    virtual Qt::CursorShape cursor() const;

    ToolManager* getManager() const;
    CGlArea* getGlArea() const;

    bool isActive();
    virtual void activate();
    virtual void deactivate();

    virtual void  handleMousePressEvent  ( QMouseEvent * ) = 0;
    virtual void  handleMouseMoveEvent   ( QMouseEvent * ) = 0;
    virtual void  handleMouseReleaseEvent( QMouseEvent * ) = 0;
    virtual void  handleWheelEvent       ( QWheelEvent * ) = 0;

protected:
    explicit AbstractTool(ToolManager* manager);

};

inline ToolManager* AbstractTool::getManager() const {
    return static_cast<ToolManager*>(parent());
}

inline CGlArea* AbstractTool::getGlArea() const {
    ToolManager* manager = getManager();
    return manager->getGlArea();
}

inline Qt::CursorShape AbstractTool::cursor() const {
    return Qt::ArrowCursor;
}

}

#endif // ABSTRACT_TOOL_H
