#ifndef DRIVERS_PLUGIN_H
#define DRIVERS_PLUGIN_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/plugins/interfaces.h>

namespace Q3D {

class DRIVERSSHARED_EXPORT DriversPlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.DriverInterface" FILE "driversplugin.json")
    Q_INTERFACES(Q3D::DriverInterface)


public:
    DriversPlugin();

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

}

#endif // DRIVERS_PLUGIN_H
