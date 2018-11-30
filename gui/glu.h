#ifndef GLU_H
#define GLU_H

#include <QtOpenGL>

void
gluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
           GLdouble centerx, GLdouble centery, GLdouble centerz,
           GLdouble upx, GLdouble upy, GLdouble upz );

#endif // GLU_H
