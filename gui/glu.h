#ifndef GLU_H
#define GLU_H

#include <QtOpenGL>


namespace Q3D {

class GLu {

public:
    static void lookAt(
        GLdouble eyex, GLdouble eyey, GLdouble eyez,
        GLdouble centerx, GLdouble centery, GLdouble centerz,
        GLdouble upx, GLdouble upy, GLdouble upz );


private:
    static void __gluMakeIdentityf(GLdouble m[16]);

};

}
#endif // GLU_H
