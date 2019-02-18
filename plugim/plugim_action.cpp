#include "plugim_action.h"

#include <QAction>

#include <q3d/cube/cube_model.h>
#include <q3D/model/process_mgr.h>
#include <q3D/plugim/plugim_process.h>

#include <QDebug>

namespace Q3D {
PlugImAction::PlugImAction(const QString& exe_file, QObject* parent) :
    PluginAction(PluginAction::MENU_ACTION, parent)
{
    exe_ = QFileInfo(exe_file);
    description_ = exe_.baseName();
    QIcon icon1;
    icon1.addFile(QLatin1String(":/plugim.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}

bool PlugImAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool PlugImAction::execute(Model* model){

    ProcessInfo pi( description_ );
    pi.setModel(model);
    pi.setProcessExe( exe_ );
    pi.addParam("IN", "C:\\TEMP\\In.fda");
    pi.addParam("OUT", "C:\\TEMP\\Out.fda" );
    pi.addParam("flow", "true");
    pi.addParam("sigmaR", 49);
    pi.addParam("sigmaS", 20);

    PlugImProcessBuilder builder;
    ProcessManager::instance()->submitProcess(pi, builder);

    return true;
}


}
