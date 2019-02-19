#include "plugim_plugin.h"

#include <QDebug>
#include <QDir>

#include <q3D/plugim/plugim_action.h>

namespace Q3D {
/**********************************************/

PlugImActionPlugin::PlugImActionPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> PlugImActionPlugin::getActions() const {

    QDir plugin_dir( QString("%1/plugim/volumeBilateral3DSEP").arg(QDir::currentPath())  );
    QFileInfo fi( plugin_dir, "volumeBilateral3DSEP.exe");
    QList<PluginAction*> actions;
    actions.append(new PlugImAction(fi.absoluteFilePath()));
    return actions;
}


/**********************************************/

PlugImCollection::PlugImCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = nullptr;
    action_interface_ = new PlugImActionPlugin(this);
}


}