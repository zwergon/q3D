#include "plugin_collection.h"

#include <q3D/plugins/plugin_action.h>

namespace Q3D {
PluginCollection::PluginCollection(QObject* parent) :
    QObject(parent),
    action_interface_(nullptr),
    driver_interface_(nullptr)
{
}

PluginCollection::~PluginCollection(){
}

void PluginCollection::start(){}

ActionInterface* PluginCollection::getActionPlugin() {
    return action_interface_;
}

DriverInterface* PluginCollection::getDriverPlugin(){
    return driver_interface_;
}

void PluginCollection::getActions( int action_type, QList<QAction*>& actions ) const {

    if ( action_interface_ != nullptr ){
        foreach( auto action, action_interface_->getActions()){
            if ( action->getType() == action_type ){
                actions.append(action->getAction());
            }
        }
    }
}

void PluginCollection::end(){}

}
