/**
 * Copyright (C) 2008-@year@ by IFP-GROUP.
 * All rights reserved.
 *
 * IFP Headquarters:
 * 1 & 4, avenue de Bois-Preau
 * 92852 Rueil-Malmaison Cedex - France
 *
 * \author Lecomte Jean-Francois
 *
 * $Id: main.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */


#include <QApplication>
#include <QtOpenGL>
#include <QtPlugin>

#include <q3D/gui/main_window.h>
#include <q3D/plugins/plugins.h>
#include <q3D/plugins/interfaces.h>
#include <q3D/model/model_mgr.h>
#include <q3D/model/model_driver_mgr.h>

using namespace Q3D;

int main(int argc, char **argv)
{
	
	QApplication::setColorSpec( QApplication::CustomColor );
	QApplication a(argc,argv);

	if ( !QGLFormat::hasOpenGL() ) {
		qWarning( "This system has no OpenGL support. Exiting." );
		return -1;
	}

    QStringList fileNames = Plugins::instance()->get_plugins();

    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);

        PluginCollectionInterface* plugin_collection =
                qobject_cast<PluginCollectionInterface*>(loader.instance());
        if ( nullptr != plugin_collection ){

            foreach( auto p, plugin_collection->plugins()){
                DriverInterface* driver =
                        qobject_cast<DriverInterface*>(p);
                if ( nullptr != driver){
                    QListIterator<QString> itd( driver->drivers() );
                    while( itd.hasNext() ){
                        const QString& driver_name = itd.next();
                        ModelDriverManager::instance()->registerDriver( driver->driver(driver_name) );
                    }
                }

            }
        }

    }
		

    CGlWindow w;
	w.resize( 550, 350 );
	w.show();
	return a.exec();
}

