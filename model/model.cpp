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
 * $Id: model.cpp 7 2008-08-01 13:07:36Z lecomtje $
 */
#include "model.h"

#include <QMenu>

namespace Q3D {


void Model::setDriver( ModelDriver *driver ){
	driver_ = driver;
}

void Model::update(){
    //notify outside this model was changed.
    emit modelUpdated( this );
}

void Model::popupMenu(QMenu *){
    //do nothing so far.
}


}


