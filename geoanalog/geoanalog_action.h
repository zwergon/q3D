#ifndef GEOANALOG_ACTION_H
#define GEOANALOG_ACTION_H

#include <q3D/geoanalog/geoanalog_global.h>

#include <q3D/plugins/plugin_action.h>
#include <q3D/drivers/cube/mongo_cube_driver.h>


class GEOANALOGSHARED_EXPORT GeoanalogAction : public Q3D::PluginAction
{
    Q_OBJECT
public:
    explicit GeoanalogAction(QObject* parent=nullptr);

private slots:
    void onActionTriggered();

};

#endif // GEOANALOG_ACTION_H
