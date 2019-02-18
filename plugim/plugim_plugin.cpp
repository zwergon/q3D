#include "plugim_plugin.h"

#include <q3D/plugim/plugim_action.h>

namespace Q3D {
/**********************************************/

PlugImActionPlugin::PlugImActionPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> PlugImActionPlugin::getActions() const {
    QList<PluginAction*> actions;
    actions.append(
      new PlugImAction("C:\\Users\\lecomtje\\Desktop\\Plugim\\PlugIns\\volumeBilateral3DSEP\\volumeBilateral3DSEP.exe")
                );
    return actions;
}


/**********************************************/

PlugImCollection::PlugImCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = nullptr;
    action_interface_ = new PlugImActionPlugin(this);
}


}
