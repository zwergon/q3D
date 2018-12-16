#include "model_mgr.h"

#include <q3D/model/model_open_info.h>
#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_driver.h>

namespace Q3D {

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

    return 0;
}


void ModelManager::removeModel( Model* model ){

    if (models_.contains(model) ){
        emit modelRemoved( model );

        models_.removeAll( model );
        delete model;
    }

}

}
