#include "abstract_tool.h"

namespace Q3D {

AbstractTool::AbstractTool(){}

AbstractTool::~AbstractTool(){}

bool AbstractTool::isActive(){
    return true;
}

void AbstractTool::activate(){}

void AbstractTool::deactivate(){}


}
