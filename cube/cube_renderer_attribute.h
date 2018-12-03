#ifndef CUBE_RENDERER_ATTRIBUTE_H
#define CUBE_RENDERER_ATTRIBUTE_H

#include <q3D/cube/cube_global.h>

#include <q3D/model/renderer_attribute.h>

namespace Q3D {


class CUBESHARED_EXPORT CubeRendererAttribute : public RendererAttribute
{
public:
    CubeRendererAttribute( ModelRenderer* renderer );

    virtual RendererAttributeDialog* createDialog( QWidget* parent = nullptr );

    int getCursorX() const;
    int getCursorY() const;
    int getCursorZ() const;

    void setCursorX( int cursor );
    void setCursorY( int cursor );
    void setCursorZ( int cursor );

private:
    int cursor_x_;
    int cursor_y_;
    int cursor_z_;
};

inline int CubeRendererAttribute::getCursorX() const {
    return cursor_x_;
}

inline int CubeRendererAttribute::getCursorY() const {
    return cursor_y_;
}

inline int CubeRendererAttribute::getCursorZ() const {
    return cursor_z_;
}



}


#endif // CUBE_RENDERER_ATTRIBUTE_H
