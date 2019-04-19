#include "cube_actions.h"

#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_mgr.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/cube/wavelet.h>
#include <q3D/cube/cube_api.h>

#include <q3D/drivers/cube/fda_cube_driver.h>

namespace Q3D {

CubeWaveAction::CubeWaveAction(QObject* parent)
    : PluginAction(PluginAction::MENU_ACTION, parent){
    description_ = "Haar decomposition";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/FlatSVG/scalable/actions/exec.svg"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}

bool CubeWaveAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool CubeWaveAction::execute(Model* model){

    CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
    if ( nullptr == cube_model ){
        return false;
    }

    Cube& src = cube_model->cube();

    Cube* dst = Cube::create(Cube::FLOAT, true);
    dst->copy(&src);

    Wavelet wavelet(dst, Wavelet::HAAR_TYPE);
    wavelet.transform(true);

    CubeModel* dst_cube_model = new CubeModel;
    dst_cube_model->setCube(dst);
    dst_cube_model->setDriver(model->driver());
    dst_cube_model->setObjectName(QString("%1_Haar").arg(cube_model->objectName()));
    dst_cube_model->update();

    ModelManager::instance()->addModel(dst_cube_model);

    return true;
}


CubeResampleAction::CubeResampleAction(QObject* parent)
    : PluginAction(PluginAction::MENU_ACTION, parent){
    description_ = "Resample";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/FlatSVG/scalable/actions/exec.svg"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}

bool CubeResampleAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool CubeResampleAction::execute(Model* model){
    CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
    if ( nullptr == cube_model ){
        return false;
    }

    Cube& src = cube_model->cube();

    Cube* dst = Cube::create(src.type(), true);

    int sx = 2;
    CubeAPI::resample(&src, sx, sx, sx, dst);

    CubeModel* dst_cube_model = new CubeModel;
    dst_cube_model->setCube(dst);
    dst_cube_model->setDriver(model->driver());
    dst_cube_model->setObjectName(QString("%1_%2").arg(cube_model->objectName()).arg(sx));
    dst_cube_model->update();

    ModelManager::instance()->addModel(dst_cube_model);
}

}
