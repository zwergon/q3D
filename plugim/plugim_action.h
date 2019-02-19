#ifndef PLUGIM_ACTION_H
#define PLUGIM_ACTION_H

#include <q3D/plugim/plugim_global.h>

#include <QFileInfo>

#include <q3D/plugins/plugin_action.h>

namespace Q3D {

class PLUGIMSHARED_EXPORT PlugImAction : public PluginAction
{
    Q_OBJECT
public:
    PlugImAction(const QString& exe_file, QObject* parent = nullptr);

    virtual bool canWorkOn(Model*) const override;
    virtual bool execute(Model*) override;

    QFileInfo getIniFile() const;

private:
    QFileInfo exe_;
};

}

#endif // PLUGIM_ACTION_H
