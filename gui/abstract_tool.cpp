#include "abstract_tool.h"

#include <QDebug>

namespace Q3D {

AbstractTool::AbstractTool(ToolManager* manager) : QObject(manager){}

AbstractTool::~AbstractTool(){}

bool AbstractTool::isActive(){
    return true;
}

void AbstractTool::activate(){}

void AbstractTool::deactivate(){}


}
