#ifndef GLU_H
#define GLU_H

#include <QtOpenGL>

#include <q3D/model/point3d.h>

namespace Q3D {

class GLu {

public:
    static void lookAt(
        GLdouble eyex, GLdouble eyey, GLdouble eyez,
        GLdouble centerx, GLdouble centery, GLdouble centerz,
        GLdouble upx, GLdouble upy, GLdouble upz );

    static void pickMatrix(
            GLdouble x, GLdouble y,
            GLdouble width, GLdouble height,
            GLint viewport[4]);

    static GLint project(
            GLdouble objx, GLdouble objy, GLdouble objz,
            const GLdouble modelMatrix[16],
            const GLdouble projMatrix[16],
            const GLint viewport[4],
            GLdouble *winx, GLdouble *winy, GLdouble *winz );

    static GLint unProject(
            GLdouble winx, GLdouble winy, GLdouble winz,
            const GLdouble modelMatrix[16],
            const GLdouble projMatrix[16],
            const GLint viewport[4],
            GLdouble *objx, GLdouble *objy, GLdouble *objz);




private:
    static void __makeIdentity(GLdouble m[16]);
    static void __multMatrixVecd(const GLdouble matrix[16], const GLdouble in[4], GLdouble out[4]);
    static void __multMatricesd(const GLdouble a[16], const GLdouble b[16], GLdouble r[16]);
    static GLint __invertMatrixd(const GLdouble m[16], GLdouble invOut[16]);


};

}
#endif // GLU_H
