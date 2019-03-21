#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <q3D/cube/cube_global.h>

#include <QtOpenGL>

#include <q3D/model/renderer.h>
#include <q3D/model/picking.h>

#include <q3D/cube/cube.h>

namespace Q3D {

class CubeRendererAttribute;

class CUBESHARED_EXPORT CubeRenderer : public ModelRenderer
{
    Q_OBJECT

public:
    enum Slice {
        XZ = 0,
        YZ,
        XY,
        LAST
    };

    CubeRenderer();
    virtual ~CubeRenderer();

    virtual void init();
    virtual void update(RendererArea*) override;
    virtual void draw(RendererArea*) override;
    virtual void clean(RendererArea*) override;

    virtual void pick(Pick& pick) override;

protected:
    virtual RendererAttribute* createAttribute();

private:
    void createTexture(const Cube& cube, GLuint* tex, int slice);
    void createUVQuad(const Cube& cube, GLuint* tex, RendererArea*);


private:
    GLuint* texture_;
};

class CUBESHARED_EXPORT CubePickInfo : public PickInfo {
public:
    virtual QString toString(const Pick &) const;

};

}

#endif // CUBE_RENDERER_H
