#ifndef WAVE97_H
#define WAVE97_H


#include <cube/cube_global.h>

extern "C" {

typedef enum WaveletType_ {
    HAAR_TYPE,
    DB4_TYPE,
    CDF97_TYPE
} WaveletType;

CUBESHARED_EXPORT float* extract_approximation( int nx, int ny, int nz, float* data, int& n1, int& n2, int& n3);
CUBESHARED_EXPORT void wave_transform( int nx, int ny, int nz, float* data, bool forward, int type );
CUBESHARED_EXPORT void cdf97_step_1D(double* input, int length, bool forward);
CUBESHARED_EXPORT void haar_step_1D(double* input, int length, bool forward);

}

#endif // WAVE97_H
