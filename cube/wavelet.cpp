#include "wavelet.h"


Wavelet::Wavelet(Cube* cube, int type)
    : cube_(cube),
      wave_(nullptr)
{
    switch(type){
    default:
    case HAAR_TYPE:
        wave_ = new Haar;
        break;
    case CDF97_TYPE:
        wave_ = new CDF97;
        break;
    case DB4_TYPE:
        wave_ = new DB4;
        break;
    }
}

Wavelet::~Wavelet(){
    if ( nullptr != wave_ ){
        delete wave_;
    }
}

bool Wavelet::transform( bool forward ){

    wave_->forward = forward;
    if ( forward ){
        forward_transform();
    }
    else {
        reverse_transform();
    }

    return true;
}

Cube* Wavelet::approximation() const {
    Cube* approx_cube = Cube::create(cube_->type(), true);
    uint32_t n1 = ( cube_->nx() + (( cube_->nx()%2 == 1 )? 1 : 0)) >> 1;
    uint32_t n2 = ( cube_->ny() + (( cube_->ny()%2 == 1 )? 1 : 0)) >> 1;
    uint32_t n3 = ( cube_->nz() + (( cube_->nz()%2 == 1 )? 1 : 0)) >> 1;
    uint32_t idx[] = {0, n1, 0, n2, 0, n3};

    approx_cube->extract(cube_, idx);
    return approx_cube;
}


void Wavelet::forward_transform(){

    uint32_t nx = cube_->nx();
    uint32_t ny = cube_->ny();
    uint32_t nz = cube_->nz();
    {
        std::vector<double> vz(nz);
        for( uint32_t i=0; i<nx; i++ ){
            for( uint32_t j=0; j<ny; j++){
                for (uint32_t k=0; k<nz; k++){
                    vz[k] = cube_->value(i, j, k);
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
        for( uint32_t i=0; i<nx; i++ ){
            for( uint32_t k=0; k<nz; k++){
                for (uint32_t j=0; j<ny; j++){
                    vy[j] = cube_->value(i, j, k);
                }
                wave_->step_1D(vy.data(), ny);
                for (uint32_t j=0; j<ny; j++){
                    cube_->setValue(i, j, k, vy[j]);
                }
            }
        }
    }

    {
        std::vector<double> vx(nx);
        for( uint32_t k=0; k<nz; k++ ){
            for( uint32_t j=0; j<ny; j++){
                for (uint32_t i=0; i<nx; i++){
                    vx[i] = cube_->value(i, j, k);
                }
                wave_->step_1D(vx.data(), nx);
                for (uint32_t i=0; i<nx; i++){
                    cube_->setValue(i, j, k, vx[i]);
                }
            }
        }
    }
}

void Wavelet::reverse_transform(){

    uint32_t nx = cube_->nx();
    uint32_t ny = cube_->ny();
    uint32_t nz = cube_->nz();
    {
        std::vector<double> vx(nx);
        for( uint32_t k=0; k<nz; k++ ){
            for( uint32_t j=0; j<ny; j++){
                for (uint32_t i=0; i<nx; i++){
                    vx[i] = cube_->value(i, j, k);
                }
                wave_->step_1D(vx.data(), nx);
                for (uint32_t i=0; i<nx; i++){
                    cube_->setValue(i, j, k, vx[i]);
                }
            }
        }
    }

    {
        std::vector<double> vy(ny);
        for( uint32_t i=0; i<nx; i++ ){
            for( uint32_t k=0; k<nz; k++){
                for (uint32_t j=0; j<ny; j++){
                    vy[j] = cube_->value(i, j, k);
                }
                wave_->step_1D(vy.data(), ny);
                for (uint32_t j=0; j<ny; j++){
                    cube_->setValue(i, j, k, vy[j]);
                }
            }
        }
    }

    {
        std::vector<double> vz(nz);
        for( uint32_t i=0; i<nx; i++ ){
            for( uint32_t j=0; j<ny; j++){
                for (uint32_t k=0; k<nz; k++){
                     vz[k] = cube_->value(i, j, k);
                }

                wave_->step_1D(vz.data(), nz);
                for (uint32_t k=0; k<nz; k++){
                    cube_->setValue(i, j, k, vz[k]);
                }
            }
        }
    }
}
