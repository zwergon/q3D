#ifndef CUBE_RENDERER_ATTRIBUTE_H
#define CUBE_RENDERER_ATTRIBUTE_H

#include <q3D/drivers/drivers_global.h>

#include <q3D/model/renderer_attribute.h>

namespace Q3D {


class DRIVERSSHARED_EXPORT CubeRendererAttribute : public RendererAttribute
{
public:
    CubeRendererAttribute( ModelRenderer* renderer );

    virtual RendererAttributeDialog* createDialog( QWidget* parent = nullptr );

    int getCursorX() const;
    int getCursorY() const;
    int getCursorZ() const;

    float opacity() const;

    void setCursorX( int cursor );
    void setCursorY( int cursor );
    void setCursorZ( int cursor );

    void setOpacity( float opacity );

private:
    int cursor_x_;
    int cursor_y_;
    int cursor_z_;
    float opacity_;
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

inline float CubeRendererAttribute::opacity() const {
    return opacity_;
}


}


#endif // CUBE_RENDERER_ATTRIBUTE_H
