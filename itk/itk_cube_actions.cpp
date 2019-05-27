#include "itk_cube_actions.h"


#include <q3D/cube/cube.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/gui/params_dialog.h>




#include <QDebug>

namespace Q3D {

class MedianInfo : public ParamsInfo {
public:
    MedianInfo(){
        elements_.append( ParamsElement("Double", "x_radius", "X Radius", "3", "X Radius") );
        elements_.append( ParamsElement("Double", "y_radius", "Y Radius", "3", "Y Radius") );
        elements_.append( ParamsElement("Double", "z_radius", "Z Radius", "3", "Z Radius") );
    }
};


MedianFilterAction::MedianFilterAction(QObject* parent) :
    PluginAction(PluginAction::MENU_ACTION, parent)
{
    description_ = "Median Filter";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/itk_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}


bool MedianFilterAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool MedianFilterAction::execute(Model* model){

    MedianInfo medianParams;

    ParamsDialog dlg(medianParams.getDomDocument());

    if ( !dlg.hasParam() || (dlg.exec()  == QDialog::Accepted) ){

        double x_radius = 3.;
        double y_radius = 3.;
        double z_radius = 3.;
        foreach( ParamsElement param, dlg.getParams()) {
            if ( param.id() == "x_radius"){
                x_radius = param.value().toDouble();
            }
            else if ( param.id() == "y_radius"){
                y_radius = param.value().toDouble();
            }
            else if ( param.id() == "z_radius"){
                z_radius = param.value().toDouble();
            }
        }
        qDebug() << QString("%1 %2 %3").arg(x_radius).arg(y_radius).arg(z_radius);

        CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
        if ( nullptr == cube_model ){
            return false;
        }

        Cube& src = cube_model->cube();
        switch(src.type()){
        case Cube::FLOAT:
            filter<float>(src, x_radius, y_radius, z_radius);
            break;
        case Cube::UINT8:
            filter<uint8_t>(src, x_radius, y_radius, z_radius);
            break;
        case Cube::UINT32:
            filter<uint32_t>(src, x_radius, y_radius, z_radius);
            break;
        case Cube::DOUBLE:
            filter<double>(src, x_radius, y_radius, z_radius);
            break;
        default:
            qDebug() << "unable to process ";
            return false;
        }

        model->update();
    }

    return true;
}

}
