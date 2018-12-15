#include "plugin_action.h"

#include <QDebug>


namespace Q3D {

void PluginAction::execute( ModelManager* ) {
    qDebug() << "No implementation in " << name();
}


}
