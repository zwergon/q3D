#ifndef MONGO_LOAD_ACTION_H
#define MONGO_LOAD_ACTION_H

#include <q3D/cube/cube_global.h>

#include <q3D/plugins/plugin_action.h>

namespace Q3D {


class CUBESHARED_EXPORT MongoLoadAction : public PluginAction
{
    Q_OBJECT
public:
    explicit MongoLoadAction(QObject* parent=nullptr);

private slots:
    void onActionTriggered();

};


}
#endif // MONGO_LOAD_ACTION_H
