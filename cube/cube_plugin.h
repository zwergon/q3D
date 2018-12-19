#ifndef CUBE_PLUGIN_H
#define CUBE_PLUGIN_H

#include <q3D/cube/cube_global.h>

#include <q3D/plugins/interfaces.h>

namespace Q3D {
class CUBESHARED_EXPORT CubePlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::DriverInterface)

public:
    CubePlugin();

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

class CubePluginAction : public QObject, public PluginActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::PluginActionInterface)

public:
    explicit CubePluginAction(QObject *parent = nullptr);
    virtual QMenu* tools() const;

private slots:
    virtual void onCubeLoadMongo();
};

class CubePluginCollection : public QObject, public PluginCollectionInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.PluginInterface" FILE "cubeplugin.json")
    Q_INTERFACES(Q3D::PluginCollectionInterface)

public:
    explicit CubePluginCollection(QObject* parent = nullptr);

    virtual QList<QObject*> plugins() override;
    virtual void start() override;
    virtual void end() override;
};

}

#endif // CUBE_PLUGIN_H
