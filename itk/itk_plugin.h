#ifndef ITK_PLUGIN_H
#define ITK_PLUGIN_H

#include <q3D/itk/itk_global.h>


#include <q3D/plugins/plugin_collection.h>

namespace Q3D {

class ITKSHARED_EXPORT ItkDriversPlugin : public QObject, public DriverInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::DriverInterface)

public:
    explicit ItkDriversPlugin(QObject* parent = nullptr);

    virtual QStringList drivers() const;
    virtual ModelDriver* driver( const QString& key );

private:
    QList<ModelDriver*> drivers_;
};

class ItkActionPlugin : public QObject, public ActionInterface
{
    Q_OBJECT
    Q_INTERFACES(Q3D::ActionInterface)

public:
    explicit ItkActionPlugin(QObject *parent = nullptr);
    virtual QString name() const { return "ITK"; }
    virtual QList<PluginAction*> getActions( QObject* parent ) const;
};


class ITKSHARED_EXPORT ItkPluginCollection : public PluginCollection {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fr.org.q3D.DriverInterface" FILE "itkplugin.json")
    Q_INTERFACES(Q3D::CollectionInterface)

public:
    explicit ItkPluginCollection( QObject* parent = nullptr );

    virtual void start() override;

};

}

#endif // ITK_PLUGIN_H
