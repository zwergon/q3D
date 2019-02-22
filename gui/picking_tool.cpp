#include "picking_tool.h"

namespace Q3D {
PickingTool::PickingTool(ToolManager* manager) : AbstractTool(manager) {
}

void
PickingTool::handleMousePressEvent (QMouseEvent* ){

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
PickingTool::handleMouseReleaseEvent(QMouseEvent* ){
}

void
PickingTool::handleWheelEvent(QWheelEvent* ){
}


}
