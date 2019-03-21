#include "renderer_area.h"

namespace Q3D {


RendererArea::RendererArea(){
}


RendererArea::~RendererArea(){
    uv_quads_.clear();
}

void RendererArea::removeUVQuads(GLuint tex){
    QMutableListIterator<UVQuad> itr(uv_quads_);
    while(itr.hasNext()){
        UVQuad& q = itr.next();
        if ( q.tex == tex ){
            itr.remove();
        }
    }
}

}
