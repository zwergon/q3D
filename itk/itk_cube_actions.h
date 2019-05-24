#ifndef ITK_CUBE_ACTIONS_H
#define ITK_CUBE_ACTIONS_H

#include <q3D/itk/itk_global.h>

#include <q3D/plugins/plugin_action.h>

namespace Q3D {

class ITKSHARED_EXPORT MedianFilterAction : public PluginAction
{
    Q_OBJECT
public:
    MedianFilterAction(QObject* parent = nullptr);

    virtual bool canWorkOn(Model*) const override;
    virtual bool execute(Model*) override;
};

}

#endif // ITK_CUBE_ACTIONS_H
