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
 * $Id: model_driver_mgr.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */



#include "factory.hpp"
#include "model_driver.h"
#include "model_driver_mgr.h"

namespace Q3D {

ModelDriverManager* ModelDriverManager::instance_ = 0;

ModelDriverManager*
ModelDriverManager::instance()
{
	if ( 0 == instance_ ){
		instance_ = new ModelDriverManager();

	}
	return instance_;
}

ModelDriverManager::ModelDriverManager(){
}

ModelDriver*
ModelDriverManager::getDriver( int idriver )
{
	if( idriver < 0 || idriver >= (int)drivers_.size() )
		return 0;
	else
		return drivers_[idriver];
}

ModelDriver*
ModelDriverManager::getDriverByName( const QString& name ){
	
	for( QVector<ModelDriver*>::iterator itr = drivers_.begin();
		itr != drivers_.end();
		itr++ ){
			ModelDriver* driver = *itr;
			if ( driver->description() == name ){
				return driver;
			}
	}
	return 0;
}

void 
ModelDriverManager::registerDriver( ModelDriver* pdriver )
{
	/*
	If it is already registered, just return the existing index
	*/
	if ( 0 != getDriverByName( pdriver->description() )){
		return;
	}

	/*
	Otherwise, grows the list to hold the new one
	*/
	drivers_.push_back( pdriver );
}

}
