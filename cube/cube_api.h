#ifndef CUBE_API_H
#define CUBE_API_H

#include <cube/cube_global.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

CUBESHARED_EXPORT float* extract_approximation(
        uint32_t nx, uint32_t ny, uint32_t nz,
        float* data,
        uint32_t& n1, uint32_t& n2, uint32_t& n3
        );
CUBESHARED_EXPORT void wave_transform(
        uint32_t nx, uint32_t ny, uint32_t nz,
        float* data,
        bool forward,
        int type
        );
CUBESHARED_EXPORT void cdf97_step_1D(double* input, int length, bool forward);
CUBESHARED_EXPORT void haar_step_1D(double* input, int length, bool forward);

#ifdef __cplusplus
}
#endif

class Cube;

class CUBESHARED_EXPORT CubeAPI {
public:
    static double norm(Cube* cube);
    static void extrema(Cube* cube, double& min, double& max);
};

#endif // CUBE_API_H
