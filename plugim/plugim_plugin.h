#ifndef PLUGIM_H
#define PLUGIM_H

#include <q3D/plugim/plugim_global.h>

#include <q3D/plugins/plugin_collection.h>

namespace Q3D {

class PlugImActionPlugin : public QObject, public ActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::ActionInterface)

public:
    static QStringList getExecutables();
public:
    explicit PlugImActionPlugin(QObject *parent = nullptr);
    virtual QString name() const { return "PlugIm"; }
    virtual QList<PluginAction*> getActions( QObject* parent ) const;
};

class PlugImCollection : public PluginCollection {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.PluginInterface" FILE "plugim_plugin.json")
    Q_INTERFACES(Q3D::CollectionInterface)

public:
    explicit PlugImCollection(QObject* parent = nullptr);

};


}
#endif // PLUGIM_H
