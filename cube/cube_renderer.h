#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <q3D/cube/cube_global.h>

#include <QtOpenGL>

#include <q3D/model/renderer.h>
#include <q3D/model/picking.h>

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


    virtual void draw() override;
    virtual void pick(Pick& pick) override;

protected:
    virtual RendererAttribute* createAttribute();

private:
    void createTexture( const Cube& cube, Slice slice, GLuint tId );

};

class CUBESHARED_EXPORT CubePickInfo : public PickInfo {
public:
    virtual QString toString(const Pick &) const;

};

}

#endif // CUBE_RENDERER_H
