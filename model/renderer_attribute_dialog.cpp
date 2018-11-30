#include "renderer_attribute_dialog.h"

namespace Q3D {

RendererAttributeDialog::RendererAttributeDialog( RendererAttribute* renderer_attribute )
    : renderer_attribute_(renderer_attribute)
{
}

QDialog* RendererAttributeDialog::getDialog() {
    return nullptr;
}

}
