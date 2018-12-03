#include "cube_renderer_attribute.h"

#include <q3D/cube/cube_renderer_attribute_dlg.h>

namespace Q3D {

CubeRendererAttribute::CubeRendererAttribute(ModelRenderer* renderer)
    : RendererAttribute(renderer)
{
}

RendererAttributeDialog* CubeRendererAttribute::createDialog( QWidget* parent ){
    return new CubeRendererAttributeDlg(this, parent);
}


void CubeRendererAttribute::setCursorX(int cursor){
    cursor_x_ = cursor;
    emit attributeChanged(this);
}

void CubeRendererAttribute::setCursorY(int cursor){
    cursor_y_ = cursor;
    emit attributeChanged(this);
}

void CubeRendererAttribute::setCursorZ(int cursor){
    cursor_z_ = cursor;
    emit attributeChanged(this);
}
}
