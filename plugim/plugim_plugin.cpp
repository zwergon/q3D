#include "plugim_plugin.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

#include <q3D/plugim/plugim_action.h>

namespace Q3D {
/**********************************************/

PlugImActionPlugin::PlugImActionPlugin(QObject *parent) : QObject(parent)
{
}

QList<PluginAction*> PlugImActionPlugin::getActions( QObject* parent ) const {


    QList<PluginAction*> actions;
    for ( QString exe : getExecutables() ){
        QFileInfo fi(exe);
        actions.append(new PlugImAction(fi.absoluteFilePath(), parent));
    }

    return actions;

}

QStringList PlugImActionPlugin::getExecutables(){

    QStringList executables;
    QSettings settings( "ifp", "q3D" );

    settings.beginGroup( "executables" );
    QStringList keys = settings.childKeys();

    for( int i=0; i< keys.size(); i++ ) {
        QString lib  = settings.value( keys.at(i) ).toString() ;
        executables.append( lib );
    }
    settings.endGroup();

    return executables;
}


/**********************************************/

PlugImCollection::PlugImCollection(QObject *parent) : PluginCollection(parent){
    driver_interface_ = nullptr;
    action_interface_ = new PlugImActionPlugin(this);
}


}
