#include "cube_actions.h"

#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_mgr.h>
#include <q3D/model/params_info.h>
#include <q3D/cube/wavelet.h>
#include <q3D/cube/cube_api.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/drivers/cube/fda_cube_driver.h>
#include <q3D/gui/params_dialog.h>


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

/*------------------------------------------------*/
class SampleInfo : public ParamsInfo {
public:
    SampleInfo(){
        ParamsElement x_param("Integer", "x_factor", "X Factor", "2", "X Factor");
        x_param.addKeyValue("Max", "6");
        elements_.append( x_param );

        ParamsElement y_param("Integer", "y_factor", "Y Factor", "2", "Y Factor");
        y_param.addKeyValue("Max", "6");
        elements_.append( y_param );

        ParamsElement z_param("Integer", "z_factor", "Z Factor", "2", "Z Factor");
        z_param.addKeyValue("Max", "6");
        elements_.append( z_param );
    }
};


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

    SampleInfo sampleParams;

    ParamsDialog dlg(sampleParams.getDomDocument());

    if ( !dlg.hasParam() || (dlg.exec()  == QDialog::Accepted) ){

        int x_factor = 2;
        int y_factor = 2;
        int z_factor = 2;
        foreach( ParamsElement param, dlg.getParams()) {
            if ( param.id() == "x_factor"){
                x_factor = param.value().toInt();
            }
            else if ( param.id() == "y_factor"){
                y_factor = param.value().toInt();
            }
            else if ( param.id() == "z_factor"){
                z_factor = param.value().toInt();
            }
        }

        CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
        if ( nullptr == cube_model ){
            return false;
        }

        Cube& src = cube_model->cube();

        Cube* dst = Cube::create(src.type(), true);

        CubeAPI::resample(&src, x_factor, y_factor, z_factor, dst);

        CubeModel* dst_cube_model = new CubeModel;
        dst_cube_model->setCube(dst);
        dst_cube_model->setDriver(model->driver());
        dst_cube_model->setObjectName(QString("%1_%2_%3_%4").arg(
                                          cube_model->objectName())
                                      .arg(x_factor)
                                      .arg(y_factor)
                                      .arg(z_factor));
        dst_cube_model->update();

        ModelManager::instance()->addModel(dst_cube_model);

        return true;
    }

    return false;
}

}
