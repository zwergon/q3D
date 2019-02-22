#include "camera_tool.h"

namespace Q3D {

CameraTool::CameraTool(ToolManager* manager) : AbstractTool(manager),
    x_prec_(0),
    y_prec_(0),
    move_activated_(false)
{
}

void
CameraTool::handleMousePressEvent (QMouseEvent* mouseEvent){


    QPoint p = mouseEvent->globalPos();
    x_prec_ = p.x();
    y_prec_ = p.y();

    CGlArea* gl_area = getGlArea();
    ViewControl& view_control = gl_area->getViewControl();
    ArcBall& arc_ball = view_control.arcBall();
    arc_ball.click(mouseEvent->x(), mouseEvent->y());

    move_activated_ = true;
}

void
CameraTool::handleMouseMoveEvent (QMouseEvent* mouseEvent){

    CGlArea* gl_area = getGlArea();

    ViewControl& view_control = gl_area->getViewControl();
    if ( move_activated_ ){
        QPoint p = mouseEvent->globalPos();

        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            if ( mouseEvent->modifiers() == Qt::CTRL ){
                cameraTranslate( p.x()-x_prec_,	p.y()-y_prec_  ) ;
            }
            else {
                cameraRotate(mouseEvent->x(), mouseEvent->y());
            }
        }

        if (mouseEvent->buttons() & Qt::MidButton)
        {
            view_control.scaleZIncr( (p.y()-y_prec_) < 0 ) ;
            gl_area->updateGL();
        }

        x_prec_ = p.x();
        y_prec_ = p.y();
    }

}

void
CameraTool::handleMouseReleaseEvent( QMouseEvent* mouseEvent){
    move_activated_ = false;
}

void
CameraTool::handleWheelEvent( QWheelEvent* wevent){
    if (!move_activated_){
        cameraZoom( wevent->delta() < 0 ) ;
    }
}

void
CameraTool::cameraRotate( int x, int y ){
    CGlArea* gl_area = getGlArea();
    ViewControl& view_control = gl_area->getViewControl();
    ArcBall& arc_ball = view_control.arcBall();
    arc_ball.drag(x, y);
    gl_area->updateGL();
}

void
CameraTool::cameraZoom( bool increase ){
    CGlArea* gl_area = getGlArea();
    ViewControl& view_control = gl_area->getViewControl();
    view_control.scaleXYIncr( increase );
    view_control.scaleZIncr( increase ) ;

    gl_area->updateGL();
}

void
CameraTool::cameraTranslate( int translate_x, int translate_y ){

    CGlArea* gl_area = getGlArea();
    Point3d min, max;
    gl_area->getBoundingBox(min, max);

    ViewControl& view_control = gl_area->getViewControl();
    double dx = (double)translate_x * (max[0]- min[0]) / (double)gl_area->width()  ;
    double dy = -(double)translate_y * (max[1]- min[1]) / (double)gl_area->height() ;
    view_control.camTranslate(dx, dy, 0);

    gl_area->updateGL();
}


}
