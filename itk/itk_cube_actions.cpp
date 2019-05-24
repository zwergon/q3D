#include "itk_cube_actions.h"


#include <q3D/cube/cube.h>
#include <q3D/drivers/cube/cube_model.h>
#include <q3D/gui/params_dialog.h>

#include <q3D/itk/itk_cube_2_image.h>

#include <itkImage.h>
#include <itkImportImageFilter.h>
#include "itkMeanImageFilter.h"



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

        foreach( ParamsElement param, dlg.getParams()) {
        }

        CubeModel* cube_model = dynamic_cast<CubeModel*>(model);
        if ( nullptr == cube_model ){
            return false;
        }

        Cube& src = cube_model->cube();

        if (src.type() != Cube::FLOAT ){
            qDebug() << "unable to process ";
            return false;
        }

        using ImageType = itk::Image<float, 3>;
        using ImportFilterType = itk::ImportImageFilter< float, 3 >;

        ImageType::Pointer image = cube_2_image<float>(src);

        using FilterType = itk::MeanImageFilter<
        ImageType, ImageType >;
        FilterType::Pointer filter = FilterType::New();

        ImageType::SizeType indexRadius;
        indexRadius[0] = 3; // radius along x
        indexRadius[1] = 3; // radius along y
        indexRadius[2] = 3;
        filter->SetRadius( indexRadius );

        filter->SetInput( image );
        filter->Update();

        ImageType* output = filter->GetOutput();
        memcpy( src.data(), output->GetBufferPointer(), src.byteSize());

        model->update();
    }

    return true;
}

}
