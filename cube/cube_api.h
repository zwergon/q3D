#ifndef Q3D_CUBE_API_H
#define Q3D_CUBE_API_H

#include <q3D/cube/cube_global.h>

extern "C" {

CUBESHARED_EXPORT bool get_cube( char* key, quint8* buffer, int nx, int ny, int nz );

}

#endif // Q3D_CUBE_API_H
