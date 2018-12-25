#include "mongo_load_action.h"

#include <QAction>

#include <q3D/cube/cube_load_mongo_dlg.h>

namespace Q3D {

MongoLoadAction::MongoLoadAction(QObject*parent):PluginIOAction(parent)
{
    QIcon icon1;
    icon1.addFile(QLatin1String(":/mongodb.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_ = new QAction(icon1, "Mongo", this);

    connect(action_, &QAction::triggered, this, &MongoLoadAction::onActionTriggered );
}

QString MongoLoadAction::getDescription() const {
    return "Mongo load";
}

QAction* MongoLoadAction::getAction() const {
    return action_;
}


void MongoLoadAction::onActionTriggered(){
    CubeLoadMongoDlg dlg;
    dlg.exec();
}

}
