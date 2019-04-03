#include "cube_actions.h"

#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_mgr.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/cube/wavelet.h>

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

    ModelDriver* driver = ModelDriverManager::instance()->getDriverByName("FdaCubeDriver");
    CubeModel* dst_cube_model = new CubeModel;
    dst_cube_model->setCube(dst);
    dst_cube_model->setDriver(driver);
    dst_cube_model->setObjectName(QString("%1_Haar").arg(cube_model->objectName()));
    dst_cube_model->update();

    ModelManager::instance()->addModel(dst_cube_model);

    return true;
}

}
