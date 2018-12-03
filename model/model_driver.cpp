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
ModelDriver::rendererKeys( QList<QString>& keys ) const {
	renderer_factory_.keys( keys );
}

QString ModelDriver::defaultRendererKey() const {
    QList<QString> keys;
    renderer_factory_.keys(keys);
    if ( !keys.isEmpty()){
        return keys.at(0);
    }

    qDebug() << "no renderer associated to " << this->metaObject()->className();
    return QString();
}

ModelRenderer* ModelDriver::createDefaultRenderer( Model* model ){
    ModelRenderer* renderer = renderer_factory_.create(defaultRendererKey());
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
