#include "itk_cube_actions.h"


#include <q3D/cube/cube.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/gui/params_dialog.h>
#include <q3D/model/model_driver_mgr.h>
#include <q3D/model/model_mgr.h>


#include <itkBinaryThresholdImageFilter.h>


#include <QDebug>

namespace Q3D {

/*------------------------------------------------*/

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

    CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
    if ( nullptr == cube_model ){
        return false;
    }

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
        src.invalidate();

        model->update();
    }

    return true;
}

/*------------------------------------------------*/

class ExtractSurfInfo : public ParamsInfo {
public:
    ExtractSurfInfo(double min, double max){
        elements_.append( ParamsElement("Double", "lower_threshold", "Lower Threshold", QString::number(min), "Lower Threshold") );
        elements_.append( ParamsElement("Double", "upper_threshold", "Upper Threshold", QString::number(max), "Upper Threshold") );
    }
};

ExtractSurfAction::ExtractSurfAction(QObject* parent) :
    PluginAction(PluginAction::MENU_ACTION, parent)
{
    description_ = "Extract Surf Filter";
    QIcon icon1;
    icon1.addFile(QLatin1String(":/itk_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
    action_->setIcon(icon1);
    action_->setText(description_);

}


bool ExtractSurfAction::canWorkOn(Model* model) const {
    return nullptr != qobject_cast<CubeModel*>(model);
}


bool ExtractSurfAction::execute(Model* model){

    CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
    if ( nullptr == cube_model ){
        return false;
    }

    Cube& src = cube_model->cube();
    if ( src.type() != Cube::FLOAT ){
        qDebug() << "unable to process ";
        return false;
    }



    ModelDriver* driver = ModelDriverManager::instance()->getDriverByName("ItkMeshDriver");
    if ( nullptr == driver ){
        return false;
    }

    ExtractSurfInfo extractParams(src.min(), src.max());

    ParamsDialog dlg(extractParams.getDomDocument());

    if ( !dlg.hasParam() || (dlg.exec()  == QDialog::Accepted) ){

        double lower_threshold = 0;
        double upper_threshold = 255.;
        foreach( ParamsElement param, dlg.getParams()) {
            if ( param.id() == "lower_threshold"){
                lower_threshold = param.value().toDouble();
            }
            else if ( param.id() == "upper_threshold"){
                upper_threshold = param.value().toDouble();
            }
        }



        ItkMeshModel* mesh_model = filter(src, lower_threshold, upper_threshold);
        if ( mesh_model == nullptr ){
            return false;
        }
        mesh_model->setDriver(driver);
        mesh_model->setObjectName( QString("%1_surf_%2").arg(cube_model->objectName()).arg(lower_threshold));
        mesh_model->update();

        ModelManager::instance()->addModel(mesh_model);

        return true;
    }

    return false;
}


ItkMeshModel* ExtractSurfAction::filter( Cube& src, float lower_threshold, float upper_threshold ){
    using ImageType = itk::Image<float, 3>;
    ImageType::Pointer image = cube_2_image<float>(src);

    using BinaryThresholdFilterType = itk::BinaryThresholdImageFilter< ImageType, ImageType >;
    BinaryThresholdFilterType::Pointer threshold = BinaryThresholdFilterType::New();
    threshold->SetInput( image );
    threshold->SetLowerThreshold( lower_threshold );
    threshold->SetUpperThreshold( upper_threshold );
    threshold->SetOutsideValue( 0. );
    threshold->SetInsideValue( 255. );

    using MeshSourceType = itk::BinaryMask3DMeshSource< ImageType, MeshType >;
    MeshSourceType::Pointer meshSource = MeshSourceType::New();
    meshSource->SetObjectValue( 255. );
    meshSource->SetInput( threshold->GetOutput() );
    meshSource->Update();


    ItkMeshModel* mesh_model = new ItkMeshModel();
    mesh_model->setMesh(meshSource->GetOutput());

    return mesh_model;

}


}
