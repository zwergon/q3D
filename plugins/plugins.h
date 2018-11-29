#ifndef PLUGINS_H
#define PLUGINS_H

#include "plugins_global.h"

#include <QStringList>

namespace Q3D {


class PLUGINSSHARED_EXPORT Plugins
{
    
public:
    static Plugins* instance();

    void add_plugin( const QString& filename );
    void remove_plugin( const QString& fileName );
    QStringList get_plugins();

private:
    static Plugins* instance_;
};

}

#endif // PLUGINS_H
