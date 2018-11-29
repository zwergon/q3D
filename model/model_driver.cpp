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
 * $Id: model_driver.cpp 6 2008-07-31 15:25:41Z lecomtje $
 */

#include "model_driver.h"

namespace Q3D {


ModelDriver::ModelDriver()
{
}

ModelDriver::~ModelDriver(){
}

void 
ModelDriver::rendererKeys( QList<QString>& keys ){
	renderer_factory_.keys( keys );
}

ModelRenderer* ModelDriver::createDefaultRenderer( Model* model ){
	ModelRenderer* renderer = renderer_factory_.createDefault();
	renderer->setModel( model );
	renderer->init();

	return renderer;

}

ModelRenderer* ModelDriver::createRenderer( Model* model, const QString& rendererName ){
	ModelRenderer* renderer = renderer_factory_.create(rendererName);
	renderer->setModel( model );
	renderer->init();

	return renderer;
}

}
