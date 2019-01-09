#include "tool_manager.h"

#include <q3D/gui/abstract_tool.h>
#include <QDebug>

namespace Q3D {

ToolManager::ToolManager(QObject *parent) :
    QObject(parent),
    action_group_(new QActionGroup(this))
{
    connect(action_group_, &QActionGroup::triggered,
            this, &ToolManager::onActionGroupTriggered);
}

ToolManager::~ToolManager(){
    //ToolManager has ownership for all tools.
    foreach( AbstractTool* tool, action_tool_map_.values()){
        if (tool->isActive()){
            tool->deactivate();
        }
        delete tool;
    }
}

QList<QAction*> ToolManager::getActions() const {
    return action_tool_map_.keys();
}

void ToolManager::registerTool(QAction *action, AbstractTool *tool){
    action_group_->addAction(action);
    action_tool_map_.insert(action, tool);
}

void ToolManager::onActionGroupTriggered(QAction *action){
    qDebug() << "onActionGroupTriggered " << action->objectName();
    emit toolSelected(action_tool_map_[action]);
}

}


