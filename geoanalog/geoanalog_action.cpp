#include "geoanalog_action.h"


#include <q3D/geoanalog/geoanalog_mongo_dlg.h>



GeoanalogAction::GeoanalogAction(QObject*parent):
    Q3D::PluginAction(PluginAction::IO_ACTION, parent)
{
    description_ = "Geoanalog load";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/geoanalog.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText("Geoanalog");

    connect(action_, &QAction::triggered, this, &GeoanalogAction::onActionTriggered );
}

void GeoanalogAction::onActionTriggered(){
    GeoAnalogMongoDlg dlg;
    dlg.exec();
}
