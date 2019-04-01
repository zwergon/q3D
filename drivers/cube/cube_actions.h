#ifndef CUBE_ACTIONS_H
#define CUBE_ACTIONS_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/plugins/plugin_action.h>

namespace Q3D {

class CubeWaveAction : public PluginAction
{
    Q_OBJECT
public:
    CubeWaveAction(QObject* parent);

    virtual bool canWorkOn(Model*) const override;
    virtual bool execute(Model*) override;

};

}

#endif // CUBE_ACTIONS_H
