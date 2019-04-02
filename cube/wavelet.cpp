#include "wavelet.h"


Wavelet::Wavelet(Cube* cube, int type)
    : cube_(cube),
      wave_()
{
    switch(type){
    default:
    case HAAR_TYPE:
        wave_.reset(new Haar);
        break;
    case CDF97_TYPE:
        wave_.reset(new CDF97 );
        break;
    case DB4_TYPE:
        wave_.reset(new DB4);
        break;
    }
}

bool Wavelet::transform( bool forward ){

    if ( forward ){
        forward_transform();
    }
    else {
        reverse_transform();
    }

    return true;
}


void Wavelet::forward_transform(){

    int nx = cube_->getNx();
    int ny = cube_->getNy();
    int nz = cube_->getNz();
    {
        std::vector<double> vz(nz);
        for( int i=0; i<nx; i++ ){
            for( int j=0; j<ny; j++){
                for (int k=0; k<nz; k++){
                    vz[k] = cube_->getValue(i, j, k);
                }

                wave_->step_1D(vz.data(), nz);
                for (int k=0; k<nz; k++){
                    cube_->setValue(i, j, k, vz[k]);
                }
            }
        }
    }

    {
        std::vector<double> vy(ny);
        for( int i=0; i<nx; i++ ){
            for( int k=0; k<nz; k++){
                for (int j=0; j<ny; j++){
                    vy[j] = cube_->getValue(i, j, k);
                }
                wave_->step_1D(vy.data(), ny);
                for (int j=0; j<ny; j++){
                    cube_->setValue(i, j, k, vy[j]);
                }
            }
        }
    }

    {
        std::vector<double> vx(nx);
        for( int k=0; k<nz; k++ ){
            for( int j=0; j<ny; j++){
                for (int i=0; i<nx; i++){
                    vx[i] = cube_->getValue(i, j, k);
                }
                wave_->step_1D(vx.data(), nx);
                for (int i=0; i<nx; i++){
                    cube_->setValue(i, j, k, vx[i]);
                }
            }
        }
    }
}

void Wavelet::reverse_transform(){

    int nx = cube_->getNx();
    int ny = cube_->getNy();
    int nz = cube_->getNz();
    {
        std::vector<double> vx(nx);
        for( int k=0; k<nz; k++ ){
            for( int j=0; j<ny; j++){
                for (int i=0; i<nx; i++){
                    vx[i] = cube_->getValue(i, j, k);
                }
                wave_->step_1D(vx.data(), nx);
                for (int i=0; i<nx; i++){
                    cube_->setValue(i, j, k, vx[i]);
                }
            }
        }
    }

    {
        std::vector<double> vy(ny);
        for( int i=0; i<nx; i++ ){
            for( int k=0; k<nz; k++){
                for (int j=0; j<ny; j++){
                    vy[j] = cube_->getValue(i, j, k);
                }
                wave_->step_1D(vy.data(), ny);
                for (int j=0; j<ny; j++){
                    cube_->setValue(i, j, k, vy[j]);
                }
            }
        }
    }

    {
        std::vector<double> vz(nz);
        for( int i=0; i<nx; i++ ){
            for( int j=0; j<ny; j++){
                for (int k=0; k<nz; k++){
                     vz[k] = cube_->getValue(i, j, k);
                }

                wave_->step_1D(vz.data(), nz);
                for (int k=0; k<nz; k++){
                    cube_->setValue(i, j, k, vz[k]);
                }
            }
        }
    }
}
