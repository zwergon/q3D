#include "glu.h"

#include <q3D/model/point3d.h>


namespace Q3D {
/*
** Make m an identity matrix
*/
void GLu::__gluMakeIdentityf(GLdouble m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

/**
 * @brief gluLookAt
 * The implementation of gluLookAt from http://www.mesa3d.org to avoid link with GLU lib.
 */
void GLu::lookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
           GLdouble centerx, GLdouble centery, GLdouble centerz,
           GLdouble upx, GLdouble upy, GLdouble upz )
{
    Q3D::Point3d forward, up;
    GLdouble m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    Q3D::normalize<double>(forward);

    /* Side = forward x up */
    Q3D::Point3d side = Q3D::cross<double>(forward, up);
    Q3D::normalize<double>(side);

    /* Recompute up as: up = side x forward */
    up = Q3D::cross<double>(side, forward);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixd(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}

}
