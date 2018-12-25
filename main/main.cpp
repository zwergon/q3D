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
#include <QtPlugin>
#include <QList>
#include <QtOpenGL>

#include <q3D/gui/main_window.h>
#include <q3D/plugins/plugins.h>
#include <q3D/plugins/plugin_collection.h>
#include <q3D/model/model_mgr.h>
#include <q3D/model/model_driver_mgr.h>

using namespace Q3D;

void extractDrivers(PluginCollection* plugin_collection)
{
    ModelDriverManager* mdm = ModelDriverManager::instance();
    DriverInterface* driver = plugin_collection->getDriverPlugin();
    if ( nullptr != driver){
        foreach( auto driver_name, driver->drivers() ){
            mdm->registerDriver( driver->driver(driver_name) );
        }
    }
}


int main(int argc, char **argv)
{
	QApplication::setColorSpec( QApplication::CustomColor );
	QApplication a(argc,argv);

	if ( !QGLFormat::hasOpenGL() ) {
		qWarning( "This system has no OpenGL support. Exiting." );
		return -1;
	}

    QList<PluginCollection*> plugins; ;
    QStringList fileNames = Plugins::instance()->get_plugins();
    foreach (QString fileName, fileNames) {
        QPluginLoader loader(fileName);
        qDebug() << "try to load " << fileName;
        PluginCollection* plugin_collection =
                qobject_cast<PluginCollection*>(loader.instance());

        if ( nullptr != plugin_collection ){
            plugins.append(plugin_collection);
            plugin_collection->start();
            extractDrivers(plugin_collection);
        }

    }
		
    CGlWindow w;
	w.resize( 550, 350 );
	w.show();
    int result = a.exec();

    foreach( auto p, plugins ) {
        p->end();
    }

    return result;
}

