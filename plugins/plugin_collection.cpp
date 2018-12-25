#include "plugin_collection.h"

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

void PluginCollection::end(){}

}
