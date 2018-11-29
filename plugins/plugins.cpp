#include "plugins.h"

#include <QSettings>
#include <QFileInfo>
#include <QDebug>

namespace Q3D {

Plugins* Plugins::instance_ = 0;

Plugins* Plugins::instance() {
    if ( instance_ == 0 ){
        instance_ = new Plugins;
    }

    return instance_;
}


void Plugins::add_plugin( const QString& fileName ){
    QFileInfo fi(fileName);
    QSettings settings( "ifp", "q3D" );

    settings.beginGroup( "plugins" );
    settings.setValue( fi.baseName(), fileName );
    settings.endGroup();
}

void Plugins::remove_plugin( const QString& fileName ){
    QFileInfo fi(fileName);
    QSettings settings( "ifp", "q3D" );

    settings.beginGroup( "plugins" );
    settings.remove( fi.baseName() );
    settings.endGroup();
}

QStringList Plugins::get_plugins(){

    QStringList filenames;
    QSettings settings( "ifp", "q3D" );

    settings.beginGroup( "plugins" );
    QStringList keys = settings.childKeys();

    for( int i=0; i< keys.size(); i++ ) {
        QString lib  = settings.value( keys.at(i) ).toString() ;
        filenames.append( lib );
    }
    settings.endGroup();

    return filenames;
}


}
