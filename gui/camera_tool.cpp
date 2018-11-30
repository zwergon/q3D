#include "camera_tool.h"

namespace Q3D {

CameraTool::CameraTool() :
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

    move_activated_ = true;
}

void
CameraTool::handleMouseMoveEvent (QMouseEvent* mouseEvent){

    ViewControl& view_control = gl_area_->getViewControl();
    if ( move_activated_ ){
        QPoint p = mouseEvent->globalPos();

        if (mouseEvent->buttons() & Qt::LeftButton)
        {
            if ( mouseEvent->modifiers() == Qt::CTRL ){
                cameraTranslate( p.y()-y_prec_,	p.x()-x_prec_  ) ;
            }
            else {
                cameraRotate( p.y()-y_prec_,	p.x()-x_prec_ ) ;
            }
        }

        if (mouseEvent->buttons() & Qt::MidButton)
        {
            view_control.scaleZIncr( (p.y()-y_prec_) < 0 ) ;
            gl_area_->updateGL();
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
CameraTool::cameraRotate( int angle_x, int angle_y ){
    ViewControl& view_control = gl_area_->getViewControl();
    view_control.angleTranslate(  angle_x,	angle_y ) ;
    gl_area_->updateGL();
}

void
CameraTool::cameraZoom( bool increase ){
    ViewControl& view_control = gl_area_->getViewControl();
    view_control.scaleXYIncr( increase );
    view_control.scaleZIncr( increase ) ;

    gl_area_->updateGL();
}

void
CameraTool::cameraTranslate( int translate_y, int translate_x ){

    Point3f min, max;
    gl_area_->getBoundingBox(min, max);

    ViewControl& view_control = gl_area_->getViewControl();
    double dx =  (double)translate_x / (double)gl_area_->width()  * (max[0]- min[0]);
    double dy = -(double)translate_y / (double)gl_area_->height() * (max[1]- min[1]);
    view_control.camTranslate(dx, dy, 0);

    gl_area_->updateGL();
}


}
