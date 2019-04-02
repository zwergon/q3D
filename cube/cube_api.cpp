#include "cube_api.h"


#include <vector>
#include <iostream>

#include <cube/cube.h>
#include <cube/wavelet.h>
#include <algo/wave1d.h>

float* extract_approximation( int nx, int ny, int nz, float* data, int& n1, int& n2, int& n3){
    n1 = .5*( nx + (( nx%2 == 1 )? 1 : 0));
    n2 = .5*( ny + (( ny%2 == 1 )? 1 : 0));
    n3 = .5*( nz + (( nz%2 == 1 )? 1 : 0));

    CubeF cube_src(false);
    cube_src.setSize(nx, ny, nz);
    cube_src.setData(data);


    CubeF cube(false);
    cube.setSize(n1, n2, n3);
    cube.setData(new float[n1*n2*n3]);


    for( int k=0; k<n3; k++ ){
        for( int j=0; j<n2; j++ ){
            for(int i=0; i<n1; i++){
                cube.setValue( i, j ,k, cube_src.getValue(i,j,k));
            }
        }
    }

    return cube.data();
}

void wave_transform( int nx, int ny, int nz, float* data, bool forward, int type ){

   CubeF cube(false);
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
