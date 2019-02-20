#include "plugim_action.h"

#include <QAction>
#include <QMessageBox>

#include <q3d/cube/cube_model.h>

#include <q3D/model/process_mgr.h>
#include <q3D/plugim/plugim_process.h>
#include <q3D/plugim/plugim_dialog.h>

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

QFileInfo PlugImAction::getIniFile() const {
    return QFileInfo(exe_.absoluteDir(), QString("%1INI.xml").arg(exe_.baseName()));
}

bool PlugImAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool PlugImAction::execute(Model* model){

    QFileInfo ini_file = getIniFile();
    QFile file( ini_file.absoluteFilePath() );
    if ( !file.open(QFile::ReadOnly | QFile::Text)){
        qCritical() << "unable to read xml file" << ini_file.fileName();
        file.close();
        return false;
    }

    QDomDocument doc(ini_file.baseName());
    if ( !doc.setContent(&file)){
        file.close();
        return false;
    }

    file.close();

    PlugImDialog dlg(doc);
    if ( dlg.exec()  == QDialog::Accepted ){

        ProcessInfo pi( description_ );
        pi.setModel(model);
        pi.setProcessExe( exe_ );

        QFileInfo file_in(QDir::temp(), "In.fda");
        pi.addParam("IN", file_in.absoluteFilePath());

        QFileInfo file_out(QDir::temp(), QString("%1_filtered.fda").arg(model->objectName()));
        pi.addParam("OUT", file_out.absoluteFilePath() );
        for( auto p : dlg.getParams() ){
            pi.addParam( p.name(), p.value() );
        }

        PlugImProcessBuilder builder;
        ProcessManager::instance()->submitProcess(pi, builder);
    }

    return true;
}


}
