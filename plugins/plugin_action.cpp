#include "plugin_action.h"

#include <QDebug>


namespace Q3D {

void PluginAction::execute( ModelsData* ) {
    qDebug() << "No implementation in " << name();
}


}
