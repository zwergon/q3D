#ifndef ITK_CUBE_2_IMAGE_H
#define ITK_CUBE_2_IMAGE_H

#include <itkImage.h>
#include <itkImportImageFilter.h>

#include <q3D/cube/cube.h>

namespace Q3D {

template <typename T>
typename itk::Image<T, 3>::Pointer cube_2_image( Cube& src ){

    using ImageType = itk::Image<T, 3>;
    using ImportFilterType = itk::ImportImageFilter< T, 3 >;
    ImportFilterType::Pointer importFilter = ImportFilterType::New();

    ImportFilterType::IndexType start;
    start.Fill( 0 );

    ImportFilterType::SizeType imsize;
    const uint32_t* dim = src.dim();
    for(int i=0; i<3; i++){
        imsize[2-i] = dim[i];
    }

    ImportFilterType::RegionType region;
    region.SetIndex( start );
    region.SetSize( imsize );
    importFilter->SetRegion( region );

    itk::SpacePrecisionType origin[3];
    itk::SpacePrecisionType  spacing[3];
    for( int i =0; i<3; i++){
        origin[2-i] = src.origin()[i];
        spacing[2-i] = src.pixelSize()[i];
    }
    importFilter->SetOrigin( origin );
    importFilter->SetSpacing( spacing );

    T* localBuffer = new T[src.size()];
    memcpy(localBuffer, src.data(), src.byteSize());
    const bool importImageFilterWillOwnTheBuffer = true;
    importFilter->SetImportPointer( localBuffer, src.size(),
                                    importImageFilterWillOwnTheBuffer );

    importFilter->Update();

    return importFilter->GetOutput();
}

}
#endif // ITK_CUBE_2_IMAGE_H
