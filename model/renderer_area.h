#ifndef RENDERER_AREA_H
#define RENDERER_AREA_H

#include <q3D/model/model_scope.h>
#include <QtOpenGL>

#include <q3D/model/uv_quad.h>

namespace Q3D {

class MODELSHARED_EXPORT RendererArea {
public:
    RendererArea();
    virtual ~RendererArea();
    const QList<UVQuad>& quadList() const;

    void addUVQuad(const UVQuad& q);

    void removeUVQuads( GLuint tex_name );
    void clearUVQuads();

protected:
    QList<UVQuad> uv_quads_;
};

inline const QList<UVQuad>& RendererArea::quadList() const{
    return uv_quads_;
}

inline void RendererArea::addUVQuad(const UVQuad& q ){
    uv_quads_.append(q);
}

inline void RendererArea::clearUVQuads(){
    uv_quads_.clear();
}

}

#endif // RENDERER_AREA_H
