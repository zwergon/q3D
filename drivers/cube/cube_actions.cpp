#include "cube_actions.h"

#include <q3D/drivers/cube/cube_model.h>

namespace Q3D {

CubeWaveAction::CubeWaveAction(QObject* parent)
    : PluginAction(PluginAction::MENU_ACTION, parent){
    description_ = "Haar decomposition";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/FlatSVG/scalable/actions/exec.svg"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}

bool CubeWaveAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool CubeWaveAction::execute(Model* model){
    return true;
}

}
