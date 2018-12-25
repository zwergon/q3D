#include "plugin_action.h"

namespace Q3D {

PluginAction::PluginAction(QObject *parent) : QObject(parent){
}

PluginAction::~PluginAction(){
    //do nothing.
}

PluginIOAction::PluginIOAction(QObject *parent) : PluginAction(parent){
}


PluginToolAction::PluginToolAction(QObject *parent): PluginAction(parent){
}


}
