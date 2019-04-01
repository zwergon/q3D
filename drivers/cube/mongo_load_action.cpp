#include "mongo_load_action.h"

#include <QAction>

#include <q3D/drivers/cube/cube_load_mongo_dlg.h>

namespace Q3D {

MongoLoadAction::MongoLoadAction(QObject*parent):
    PluginAction(PluginAction::IO_ACTION, parent)
{
    description_ = "Mongo load";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/mongodb.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText("Mongo");

    connect(action_, &QAction::triggered, this, &MongoLoadAction::onActionTriggered );
}

void MongoLoadAction::onActionTriggered(){
    CubeLoadMongoDlg dlg;
    dlg.exec();
}

}
