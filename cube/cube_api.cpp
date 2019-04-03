#include "cube_api.h"


#include <vector>
#include <iostream>

#include <cube/cube.h>
#include <cube/wavelet.h>
#include <algo/wave1d.h>


/*************************************************************/

float* extract_approximation( int nx, int ny, int nz, float* data, int& n1, int& n2, int& n3){
    n1 = .5*( nx + (( nx%2 == 1 )? 1 : 0));
    n2 = .5*( ny + (( ny%2 == 1 )? 1 : 0));
    n3 = .5*( nz + (( nz%2 == 1 )? 1 : 0));

    CubeT<float> cube_src(false);
    cube_src.setSize(nx, ny, nz);
    cube_src.setData(data);


    CubeT<float> cube(false);
    int idx[] = {0, n1, 0, n2, 0, n3};
    cube.extract(&cube_src, idx);

    return reinterpret_cast<float*>(cube.data());
}

void wave_transform( int nx, int ny, int nz, float* data, bool forward, int type ){

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
  for( int i =0; i<cube->size(); i++ ){
      double v = cube->valueIdx(i);
      norm += v*v;
  }

  return norm;
}


void CubeAPI::extrema(Cube* cube, double& min, double& max){

    min = cube->valueIdx(0);
    max = min;

    for( int i =1; i<cube->size(); i++ ){
        double v = cube->valueIdx(i);
        if ( v < min ) min = v;
        if ( v > max ) max = v;
    }

}
