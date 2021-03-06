#include "cube.h"

#include <iostream>
#include <type_traits>


Cube* Cube::create(int type, bool own_memory){
    Cube* cube = nullptr;
    switch(type){
    case Cube::UINT8:
        cube = new CubeT<uint8_t>(own_memory);
        break;
    case Cube::UINT32:
        cube = new CubeT<uint32_t>(own_memory);
        break;
    case Cube::FLOAT:
        cube = new CubeT<float>(own_memory);
        break;
    case Cube::DOUBLE:
        cube = new CubeT<double>(own_memory);
        break;
    default:
       break;
    }

    return cube;
}

/*************************************************/
Cube::Cube(bool own_memory) :
    dirty_(true),
    own_memory_(own_memory),
    type_(0),
    dim_{0, 0, 0},
    pixel_{1., 1., 1.},
    origin_{0., 0., 0.},
    extrema_{0., 1.}
{
}

Cube::~Cube(){
}

void Cube::extract(Cube* src, uint32_t idx[]) {
    int n1 = idx[1] - idx[0];
    int n2 = idx[3] - idx[2];
    int n3 = idx[5] - idx[4];

    allocate(n1, n2, n3);
    for( int k=0; k<n3; k++ ){
        for( int j=0; j<n2; j++ ){
            for(int i=0; i<n1; i++){
                setValue( i, j ,k, src->value(i+idx[0], j+idx[2], k+idx[4]));
            }
        }
    }
}


void Cube::allocate(uint32_t nx, uint32_t ny, uint32_t nz){
    setSize(nx, ny, nz);
    allocate_();
}

void Cube::copy(Cube* src) {
    allocate(src->nx(), src->ny(), src->nz());

    if ( src->type() == type_ ){
        memcpy(data(), src->data(), src->byteSize());
    }
    else {
        for( uint32_t i=0; i<size(); i++ ){
            setValueIdx(i, src->valueIdx(i));
        }
    }

    dirty_ = true;
}

void Cube::add(Cube* cube){
    if (( cube->nx() != nx() ) || (cube->ny() != ny() ) || ( cube->nz() != ny() )){
        std::cerr << "unable to add two cube with different dimensions" << std::endl;
        return;
    }
    for( uint32_t i=0; i<size(); i++ ){
        setValueIdx(i, valueIdx(i) + cube->valueIdx(i));
    }

    dirty_ = true;
}

void Cube::substract(Cube* cube){
    if (( cube->nx() != nx() ) || (cube->ny() != ny() ) || ( cube->nz() != ny() )){
        std::cerr << "unable to add two cube with different dimensions" << std::endl;
        return;
    }
    for( uint32_t i=0; i<size(); i++ ){
        setValueIdx(i, valueIdx(i) - cube->valueIdx(i));
    }

    dirty_ = true;
}





