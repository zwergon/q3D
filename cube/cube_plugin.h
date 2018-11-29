#ifndef CUBE_PLUGIN_H
#define CUBE_PLUGIN_H

#include <q3D/cube/cube_global.h>

#include <q3D/plugins/interfaces.h>

namespace Q3D {
class CUBESHARED_EXPORT CubePlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.DriverInterface" FILE "cubeplugin.json")
    Q_INTERFACES(Q3D::DriverInterface)


public:
    CubePlugin();

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

}

#endif // CUBE_PLUGIN_H
