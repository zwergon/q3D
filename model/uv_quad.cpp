#include "uv_quad.h"


namespace Q3D {

/*----------------------------------------------*/

void UVPoint::init(GLfloat x, GLfloat y, GLfloat z, GLfloat u, GLfloat v){
    uv[0] = u;
    uv[1] = v;
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}


/*----------------------------------------------*/

GLfloat UVQuad::cam2center[3];
GLfloat UVQuad::modelview[16];

GLfloat scalar(GLfloat v1[], GLfloat v2[]){
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void
multMatrixVec(
        const GLfloat matrix[16],
        const GLfloat in[4],
        GLfloat out[4])
{

    for (int i=0; i<4; i++) {
        out[i] =
                in[0] * matrix[0*4+i] +
                in[1] * matrix[1*4+i] +
                in[2] * matrix[2*4+i] +
                in[3] * matrix[3*4+i];
    }
}


void UVQuad::draw() const {
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    for(int i=0; i<4; i++){
        const UVPoint& pt = pts[i];
        glTexCoord2f(pt.uv[0], pt.uv[1]);
        glVertex3f(pt.xyz[0], pt.xyz[1], pt.xyz[2]);
    }
    glEnd();
}

void UVQuad::center(GLfloat center[]) const{

    //coordinates of the object
    GLfloat c[4];
    for( int i =0; i<3; i++){
        c[i] = 0;
    }
    c[3] = 1.;

    for(int j=0; j<4; j++){
        const UVPoint& pt = pts[j];
        for(int i=0; i<3; i++){
            c[i] += .25*pt.xyz[i];
        }
    }

    //transform them in eye view.
    GLfloat c_view[4];
    multMatrixVec(modelview, c, c_view);

     for( int i =0; i<3; i++){
         center[i] = c_view[i]/c_view[3];
     }

}

bool UVQuad::lessThan(const UVQuad &q1, const UVQuad &q2){
    GLfloat c1[3];
    q1.center(c1);

    GLfloat c2[3];
    q2.center(c2);

    return scalar(c1, cam2center) < scalar(c2, cam2center);

}

}

