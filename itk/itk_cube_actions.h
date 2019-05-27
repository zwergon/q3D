#ifndef ITK_CUBE_ACTIONS_H
#define ITK_CUBE_ACTIONS_H

#include <q3D/itk/itk_global.h>

#include <q3D/plugins/plugin_action.h>

#include <q3D/itk/itk_cube_2_image.h>

#include <itkImage.h>
#include <itkMeanImageFilter.h>

namespace Q3D {

class ITKSHARED_EXPORT MedianFilterAction : public PluginAction
{
    Q_OBJECT
public:
    MedianFilterAction(QObject* parent = nullptr);

    virtual bool canWorkOn(Model*) const override;
    virtual bool execute(Model*) override;

private:
    template <typename T> void filter( Cube& cube, float x_radius, float y_radius, float z_radius );
};

template <typename T>
void MedianFilterAction::filter(Cube& src, float x_radius, float y_radius, float z_radius) {
    using ImageType = itk::Image<T, 3>;
    ImageType::Pointer image = cube_2_image<T>(src);

    using FilterType = itk::MeanImageFilter<ImageType, ImageType >;
    FilterType::Pointer filter = FilterType::New();

    ImageType::SizeType indexRadius;
    indexRadius[0] = x_radius; // radius along x
    indexRadius[1] = y_radius; // radius along y
    indexRadius[2] = z_radius;
    filter->SetRadius( indexRadius );

    filter->SetInput( image );
    filter->Update();

    ImageType* output = filter->GetOutput();
    memcpy( src.data(), output->GetBufferPointer(), src.byteSize());
}



}

#endif // ITK_CUBE_ACTIONS_H
