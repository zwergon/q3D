#include "model_mgr.h"

#include <q3D/model/model_open_info.h>
#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_driver.h>

#include <QDebug>

namespace Q3D {


ModelManager* ModelManager::instance_ = nullptr;

ModelManager*
ModelManager::instance()
{
    if ( nullptr == instance_ ){
        instance_ = new ModelManager();
    }
    return instance_;
}

Model* ModelManager::loadModel( const ModelOpenInfo& moi ){
    Model* pmodel = nullptr;
    ModelDriver* pdriver = nullptr;

    ModelDriverManager* pdriver_manager = ModelDriverManager::instance();

    /* Try each registered driver to see if this file can be opened	*/
    for (int i=0; i< pdriver_manager->getDriverCount(); i++)
    {
        pdriver = pdriver_manager->getDriver(i);
        pmodel   = pdriver->open( moi );

        /* break when first valid driver is found */
        if ( nullptr != pmodel ){
            models_.append( pmodel );

            emit modelAdded( pmodel );

            return pmodel;
        }
    }

    return nullptr;
}


void ModelManager::addModel(Model *model){

    if ( model->driver() == nullptr ){
        qDebug() << "try to add model without driver";
        return;
    }

    models_.append( model );
    emit modelAdded( model );

}


void ModelManager::removeModel( Model* model ){

    if (models_.contains(model) ){
        emit modelRemoved( model );

        models_.removeAll( model );
        delete model;
    }

}

}
