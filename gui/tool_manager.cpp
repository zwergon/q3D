#include "tool_manager.h"

#include <q3D/gui/abstract_tool.h>

namespace Q3D {

ToolManager::ToolManager(CGlArea *parent) :
    QObject(parent),
    action_group_(new QActionGroup(this))
{
    connect(action_group_, &QActionGroup::triggered,
            this, &ToolManager::onActionGroupTriggered);
}

QList<QAction*> ToolManager::getActions() const {
    return action_group_->actions();
}

void ToolManager::registerTool(QAction *action, AbstractTool *tool){
    action_group_->addAction(action);
    action_tool_map_.insert(action, tool);
}

void ToolManager::onActionGroupTriggered(QAction *action){
    emit toolSelected(action_tool_map_[action]);
}

}


