#ifndef MONGO_LOAD_ACTION_H
#define MONGO_LOAD_ACTION_H

#include <q3D/cube/cube_global.h>

#include <q3D/plugins/plugin_action.h>

namespace Q3D {


class CUBESHARED_EXPORT MongoLoadAction : public PluginIOAction
{
    Q_OBJECT

public:
    explicit MongoLoadAction(QObject* parent=nullptr);

    virtual QString getDescription() const;
    virtual QAction* getAction() const;

private slots:
    void onActionTriggered();

private:
    QAction* action_;

};


}
#endif // MONGO_LOAD_ACTION_H
