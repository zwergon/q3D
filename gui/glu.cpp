#include "glu.h"

#include <q3D/model/point3d.h>


namespace Q3D {


#define SWAP_ROWS(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }

#define MAT(m,r,c) (m)[(c)*4+(r)]

/*
** Make m an identity matrix
*/
void GLu::__makeIdentity(GLdouble m[16])
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

    __makeIdentity(&m[0][0]);
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

void GLu::pickMatrix(
        GLdouble x, GLdouble y,
        GLdouble width, GLdouble height,
        GLint viewport[4])
{
    GLdouble m[16];
    GLdouble sx, sy;
    GLdouble tx, ty;

    sx = viewport[2] / width;
    sy = viewport[3] / height;
    tx = (viewport[2] + 2.0 * (viewport[0] - x)) / width;
    ty = (viewport[3] + 2.0 * (viewport[1] - y)) / height;


    MAT(m, 0, 0) = sx;
    MAT(m, 0, 1) = 0.0;
    MAT(m, 0, 2) = 0.0;
    MAT(m, 0, 3) = tx;
    MAT(m, 1, 0) = 0.0;
    MAT(m, 1, 1) = sy;
    MAT(m, 1, 2) = 0.0;
    MAT(m, 1, 3) = ty;
    MAT(m, 2, 0) = 0.0;
    MAT(m, 2, 1) = 0.0;
    MAT(m, 2, 2) = 1.0;
    MAT(m, 2, 3) = 0.0;
    MAT(m, 3, 0) = 0.0;
    MAT(m, 3, 1) = 0.0;
    MAT(m, 3, 2) = 0.0;
    MAT(m, 3, 3) = 1.0;


    glMultMatrixd(m);
}

void
GLu::__multMatrixVecd(
        const GLdouble matrix[16],
        const GLdouble in[4],
        GLdouble out[4])
{

    for (int i=0; i<4; i++) {
        out[i] =
                in[0] * matrix[0*4+i] +
                in[1] * matrix[1*4+i] +
                in[2] * matrix[2*4+i] +
                in[3] * matrix[3*4+i];
    }
}

/*
 ** Invert 4x4 matrix.
 ** Contributed by David Moore (See Mesa bug #6748)
 */
GLint  GLu::__invertMatrixd(const GLdouble m[16], GLdouble invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
            + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
            - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
            + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
            - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
            - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
            + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
            - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
            + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
            + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
            - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
            + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
            - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
            - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
            + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
            - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
            + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return GL_FALSE;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return GL_TRUE;
}

void  GLu::__multMatricesd(const GLdouble a[16],
    const GLdouble b[16],
    GLdouble r[16])
{
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            r[i*4+j] =
                    a[i*4+0]*b[0*4+j] +
                    a[i*4+1]*b[1*4+j] +
                    a[i*4+2]*b[2*4+j] +
                    a[i*4+3]*b[3*4+j];
        }
    }
}


GLint
GLu::project(GLdouble objx, GLdouble objy, GLdouble objz,
            const GLdouble modelMatrix[16],
            const GLdouble projMatrix[16],
            const GLint viewport[4],
            GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
    double in[4];
    double out[4];

    in[0]=objx;
    in[1]=objy;
    in[2]=objz;
    in[3]=1.0;
    __multMatrixVecd(modelMatrix, in, out);
    __multMatrixVecd(projMatrix, out, in);
    if (in[3] == 0.0) return(GL_FALSE);
    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];
    /* Map x, y and z to range 0-1 */
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;

    /* Map x,y to viewport */
    in[0] = in[0] * viewport[2] + viewport[0];
    in[1] = in[1] * viewport[3] + viewport[1];

    *winx=in[0];
    *winy=in[1];
    *winz=in[2];
    return(GL_TRUE);
}

GLint
GLu::unProject(
        GLdouble winx, GLdouble winy, GLdouble winz,
        const GLdouble modelMatrix[16],
        const GLdouble projMatrix[16],
        const GLint viewport[4],
        GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
    double finalMatrix[16];
    double in[4];
    double out[4];

    __multMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__invertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    /* Map x and y from window coordinates */
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __multMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    return(GL_TRUE);
}

}
