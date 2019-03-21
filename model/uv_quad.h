#ifndef UV_QUAD_H
#define UV_QUAD_H

#include <q3D/model/model_scope.h>

#include <QtOpenGL>

namespace Q3D {


class MODELSHARED_EXPORT UVPoint {
public:
    void init(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v);

private:
    GLfloat uv[2];
    GLfloat xyz[3];

    friend class UVQuad;

};

class MODELSHARED_EXPORT UVQuad {
public:
    static GLfloat cam2center[3];
    static GLfloat modelview[16];

    int id;
    GLuint tex;
    UVPoint pts[4];

    void draw() const;

    void center(GLfloat c[3]) const;

    static bool lessThan(const UVQuad& q1, const UVQuad& q2);
};

}
#endif // UV_QUAD_H
