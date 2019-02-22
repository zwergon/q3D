#ifndef CUBE_PLUGIN_H
#define CUBE_PLUGIN_H

#include <q3D/cube/cube_global.h>

#include <q3D/plugins/plugin_collection.h>

namespace Q3D {
class CUBESHARED_EXPORT CubeDriverPlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::DriverInterface)

public:
    explicit CubeDriverPlugin(QObject* parent);

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

class CubeActionPlugin : public QObject, public ActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::ActionInterface)

public:
    explicit CubeActionPlugin(QObject *parent = nullptr);
    virtual QList<PluginAction*> getActions( QObject* parent ) const;
    virtual QString name() const { return "Cube"; }
};

class CubePluginCollection : public PluginCollection {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.PluginInterface" FILE "cubeplugin.json")
    Q_INTERFACES(Q3D::CollectionInterface)

public:
    explicit CubePluginCollection(QObject* parent = nullptr);

    virtual void start() override;
    virtual void end() override;
};

}

#endif // CUBE_PLUGIN_H
