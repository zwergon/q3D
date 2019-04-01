#include "wavelet.h"

#include <vector>
#include <iostream>

#include <algo/wave1d.h>

static int _indice(int i, int j, int k, int Nx, int Ny, int Nz){
    return k+j*Nz+i*Nz*Ny;
}

static void _forward_transform(int nx, int ny, int nz, std::unique_ptr<Wave1D>& wav, float* data)
{
    {
        std::vector<double> vz(nz);
        for( int i=0; i<nx; i++ ){
            for( int j=0; j<ny; j++){
                for (int k=0; k<nz; k++){
                    vz[k] = data[_indice(i, j, k, nx, ny, nz)];
                }

                wav->step_1D(vz.data(), nz);
                for (int k=0; k<nz; k++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vz[k];
                }
            }
        }
    }

    {
        std::vector<double> vy(ny);
        for( int i=0; i<nx; i++ ){
            for( int k=0; k<nz; k++){
                for (int j=0; j<ny; j++){
                    vy[j] = data[_indice(i, j, k, nx, ny, nz)];
                }
                wav->step_1D(vy.data(), ny);
                for (int j=0; j<ny; j++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vy[j];
                }
            }
        }
    }

    {
        std::vector<double> vx(nx);
        for( int k=0; k<nz; k++ ){
            for( int j=0; j<ny; j++){
                for (int i=0; i<nx; i++){
                    vx[i] = data[_indice(i, j, k, nx, ny, nz)];
                }
                wav->step_1D(vx.data(), nx);
                for (int i=0; i<nx; i++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vx[i];
                }
            }
        }
    }
}

static void _reverse_transform(int nx, int ny, int nz, std::unique_ptr<Wave1D>& wav, float* data)
{
    {
        std::vector<double> vx(nx);
        for( int k=0; k<nz; k++ ){
            for( int j=0; j<ny; j++){
                for (int i=0; i<nx; i++){
                    vx[i] = data[_indice(i, j, k, nx, ny, nz)];
                }
                wav->step_1D(vx.data(), nx);
                for (int i=0; i<nx; i++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vx[i];
                }
            }
        }
    }

    {
        std::vector<double> vy(ny);
        for( int i=0; i<nx; i++ ){
            for( int k=0; k<nz; k++){
                for (int j=0; j<ny; j++){
                    vy[j] = data[_indice(i, j, k, nx, ny, nz)];
                }
                wav->step_1D(vy.data(), ny);
                for (int j=0; j<ny; j++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vy[j];
                }
            }
        }
    }

    {
        std::vector<double> vz(nz);
        for( int i=0; i<nx; i++ ){
            for( int j=0; j<ny; j++){
                for (int k=0; k<nz; k++){
                    vz[k] = data[_indice(i, j, k, nx, ny, nz)];
                }

                wav->step_1D(vz.data(), nz);
                for (int k=0; k<nz; k++){
                    data[_indice(i, j, k, nx, ny, nz)] = (float)vz[k];
                }
            }
        }
    }
}

float* extract_approximation( int nx, int ny, int nz, float* data, int& n1, int& n2, int& n3){
    n1 = .5*( nx + (( nx%2 == 1 )? 1 : 0));
    n2 = .5*( ny + (( ny%2 == 1 )? 1 : 0));
    n3 = .5*( nz + (( nz%2 == 1 )? 1 : 0));

    float* cube = new float[n1*n2*n3];
    for( int k=0; k<n3; k++ ){
        for( int j=0; j<n2; j++ ){
            for(int i=0; i<n1; i++){
                cube[_indice(i,j,k,n1,n2,n3)] = data[_indice(i,j,k,nx,ny,nz)];
            }
        }
    }

    return cube;
}

void wave_transform( int nx, int ny, int nz, float* data, bool forward, int type ){

    std::unique_ptr<Wave1D> wav;
    switch(type){
    case CDF97_TYPE:
        wav.reset(new CDF97);
        break;
    case DB4_TYPE:
        wav.reset(new DB4);
        break;
    default:
    case HAAR_TYPE:
        wav.reset(new Haar);
        break;
    }

    if ( wav == nullptr ){
        std::cerr << "no wavelet is constructed with this type" << std::endl;
        return;
    }
    wav->forward = forward;

    if ( forward ){
        _forward_transform(nx, ny, nz, wav, data);
    }
    else {
        _reverse_transform(nx, ny, nz, wav, data);
    }


}

void cdf97_step_1D(double* input, int length, bool forward) {
    CDF97 cdf;
    cdf.forward = forward;
    cdf.step_1D(input, length);
}
