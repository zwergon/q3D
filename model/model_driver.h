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
 * $Id: model_driver.h 7 2008-08-01 13:07:36Z lecomtje $
 */

#ifndef Q3D_MODEL_MODEL_DRIVER_H_
#define Q3D_MODEL_MODEL_DRIVER_H_ 1

#include <QString>
#include <QList>

#include <q3D/model/model_scope.h>
#include <q3D/model/factory.hpp>
#include <q3D/model/renderer.h>
#include <q3D/model/model_open_info.h>

namespace Q3D {

class Model;

MODELSHARED_EXPORT typedef ObjectFactory<ModelRenderer, QString> ModelRendererFactory;

class MODELSHARED_EXPORT ModelDriver : public QObject
{
    Q_OBJECT

public:
    ModelDriver();
    virtual ~ModelDriver();

    const QString&       description() const;
	void                 setDescription( const QString& descr );

    ModelRendererFactory& rendererFactory();

    virtual Model*       open( const ModelOpenInfo& ) = 0;
    virtual void         save( const Model& model, const QString& filename  ) = 0;

    void                 rendererKeys( QList<QString>& keys ) const;
	int                  getRendererCount() const;
    QString              defaultRendererKey() const;
    ModelRenderer*       createDefaultRenderer( Model* );
    ModelRenderer*       createRenderer       ( Model*, const QString& );

protected:
	QString             description_;
    ModelRendererFactory renderer_factory_;

};

inline const QString& ModelDriver::description() const {
	return description_;
}

inline void ModelDriver::setDescription( const QString &descr ){
	description_ = descr;
}

inline int ModelDriver::getRendererCount() const{
	return renderer_factory_.count();
}

inline ModelRendererFactory& ModelDriver::rendererFactory() {
	return renderer_factory_;
}

}

#endif /* Q3D_MODEL_MODEL_DRIVER_H_ */

