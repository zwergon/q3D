#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <q3D/cube/cube_global.h>

#include <QtOpenGL>

#include <q3D/model/renderer.h>
#include <q3D/cube/cube.h>

namespace Q3D {

class CUBESHARED_EXPORT CubeRenderer : public ModelRenderer
{
    Q_OBJECT

public:
    enum Slice {
        XY = 0,
        XZ,
        YZ,
        LAST
    };

    CubeRenderer();
    virtual ~CubeRenderer();

    virtual void init();
    virtual void buildGlList();


    virtual void draw();

protected:
    virtual RendererAttribute* createAttribute();

private:
    void createTexture( const Cube& cube, Slice slice, GLuint tId );

private:
    int xCursor;
    int yCursor;
    int zCursor;

};

}

#endif // CUBE_RENDERER_H
