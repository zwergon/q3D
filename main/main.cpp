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
#include <q3D/model/models_data.h>
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
        DriverInterface* iDriver = qobject_cast<DriverInterface*>(loader.instance());
        if ( 0 != iDriver){
            QListIterator<QString> itd( iDriver->drivers() );
            while( itd.hasNext() ){
                const QString& driver_name = itd.next();
                ModelDriverManager::instance()->registerDriver( iDriver->driver(driver_name) );
            }
        }
    }
		
    ModelsData data;
    CGlWindow w(&data);

	w.resize( 550, 350 );
	w.show();
	return a.exec();
}

