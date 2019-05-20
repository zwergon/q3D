#include "cube_api.h"


#include <vector>
#include <iostream>

#include <cube/cube.h>
#include <cube/wavelet.h>
#include <algo/wave1d.h>


/*************************************************************/

float* extract_approximation(
        uint32_t nx, uint32_t ny, uint32_t nz,
        float* data,
        uint32_t& n1, uint32_t& n2, uint32_t& n3){
    n1 = .5*( nx + (( nx%2 == 1 )? 1 : 0));
    n2 = .5*( ny + (( ny%2 == 1 )? 1 : 0));
    n3 = .5*( nz + (( nz%2 == 1 )? 1 : 0));

    CubeT<float> cube_src(false);
    cube_src.setSize(nx, ny, nz);
    cube_src.setData(data);


    CubeT<float> cube(false);
    uint32_t idx[] = {0, n1, 0, n2, 0, n3};
    cube.extract(&cube_src, idx);

    return reinterpret_cast<float*>(cube.data());
}

void wave_transform( uint32_t nx, uint32_t ny, uint32_t nz,
                     float* data,
                     bool forward,
                     int type ){

   CubeT<float> cube(false);
   cube.setSize(nx, ny, nz);
   cube.setData(data);

   Wavelet wavelet(&cube, type);
   wavelet.transform(forward);

}

void cdf97_step_1D(double* input, int length, bool forward) {
    CDF97 cdf;
    cdf.forward = forward;
    cdf.step_1D(input, length);
}


/******************************************************/


double CubeAPI::norm(Cube* cube){

  double norm = 0;
  for( uint32_t i =0; i<cube->size(); i++ ){
      double v = cube->valueIdx(i);
      norm += v*v;
  }

  return norm;
}


void CubeAPI::extrema(Cube* cube, double& min, double& max){

    min = cube->valueIdx(0);
    max = min;

    for( uint32_t i =1; i<cube->size(); i++ ){
        double v = cube->valueIdx(i);
        if ( v < min ) min = v;
        if ( v > max ) max = v;
    }

}

bool CubeAPI::resample(Cube *src, int sx, int sy, int sz, Cube *dest){
    uint32_t nx = src->nx() / sx;
    uint32_t ny = src->ny() / sy;
    uint32_t nz = src->nz() / sz;

    dest->allocate(nx, ny, nz);
    const double* pixSize = src->pixelSize();
    const double* orig = src->origin();
    dest->setOrigin(orig[0], orig[1], orig[2]);
    dest->setPixelSize(pixSize[0]*sx, pixSize[1]*sy, pixSize[2]*sz);

    for( uint32_t k=0; k<nz; k++ ){
        for( uint32_t j=0; j< ny; j++ ){
            for( uint32_t i=0; i<nx; i++ ){
                dest->setValue(i,j,k, src->value(i*sx, j*sy, k*sz) );
            }
        }
    }

    return true;
}
