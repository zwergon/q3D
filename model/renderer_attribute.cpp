#include "renderer_attribute.h"

#include <q3D/model/renderer.h>

namespace Q3D {

RendererAttribute::RendererAttribute(ModelRenderer *parent) :
    QObject(parent)
{
}

ModelRenderer* RendererAttribute::renderer() const {
    return static_cast<ModelRenderer*>( parent() );
}

RendererAttributeDialog* RendererAttribute::createDialog( QWidget* ){
    return nullptr;
}


}
