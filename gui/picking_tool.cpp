#include "picking_tool.h"

namespace Q3D {
PickingTool::PickingTool(){
}

void
PickingTool::activate(){
    CGlArea* area = getGlArea();
    if ( nullptr != area ){
        area->setCursor(Qt::CrossCursor);
    }
}

void PickingTool::deactivate(){
    CGlArea* area = getGlArea();
    if ( nullptr != area ){
        area->setCursor(Qt::ArrowCursor);
        area->mainWindow()->statusBar()->clearMessage();
    }
}

void
PickingTool::handleMousePressEvent (QMouseEvent* mouseEvent){

}

void
PickingTool::handleMouseMoveEvent (QMouseEvent* mouseEvent){
    CGlArea* area = getGlArea();
    Pick pick;
    area->doPickingAt(mouseEvent->x(), mouseEvent->y(), pick);

    if (pick.isSucceeded()){
        area->mainWindow()->statusBar()->showMessage( pick.stringAt(0) );
    }
    else {
        area->mainWindow()->statusBar()->clearMessage();
    }
}

void
PickingTool::handleMouseReleaseEvent( QMouseEvent* mouseEvent){
}

void
PickingTool::handleWheelEvent( QWheelEvent* wevent){
}


}
