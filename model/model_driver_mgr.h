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
 * $Id: model_driver_mgr.h 6 2008-07-31 15:25:41Z lecomtje $
 */

#ifndef Q3D_MODEL_MODEL_DRIVER_MGR_H_
#define Q3D_MODEL_MODEL_DRIVER_MGR_H_ 1

#include <QString>
#include <QVector>

#include <q3D/model/model_scope.h>

namespace Q3D {

using namespace std;

class ModelDriver;

class MODELSHARED_EXPORT ModelDriverManager
{
public:
    static ModelDriverManager*  instance();

public:
	template<typename CoreDriver> 
	void registerDriver  ( const QString& );
    void registerDriver  ( ModelDriver *);
    void deregisterDriver( ModelDriver *);

	int                         getDriverCount() { return drivers_.size(); }
    ModelDriver*                getDriver( int );
    ModelDriver*                getDriverByName( const QString& );

protected:
    ModelDriverManager();

private:
    static ModelDriverManager* instance_;
    QVector<ModelDriver*>      drivers_;

};

template< typename CoreDriver>
void ModelDriverManager::registerDriver( const QString& driver_name ){

	if ( 0 == getDriverByName(driver_name) ){	
		registerDriver( new CoreDriver ); 	
	}

}

}

#endif /* Q3D_MODEL_MODEL_DRIVER_MGR_H_ */

