#include "cube.h"

#include <iostream>


Cube* Cube::create(int type, bool own_memory){
    Cube* cube = nullptr;
    switch(type){
    case Cube::UINT8:
        cube = new CubeUC(own_memory);
        break;
    case Cube::UINT32:
        cube = new CubeI32(own_memory);
        break;
    case Cube::FLOAT:
        cube = new CubeF(own_memory);
        break;
    case Cube::DOUBLE:
        cube = new CubeD(own_memory);
        break;
    default:
       break;
    }

    return cube;
}

/*************************************************/
Cube::Cube(int type, bool own_memory) :
    own_memory_(own_memory),
    type_(type),
    nx_(0),
    ny_(0),
    nz_(0),
    data_()
{
}

Cube::~Cube(){
    clean();
}

void Cube::extract(Cube* src, int idx[]) {
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

void Cube::clean(){
    if ( own_memory_&& (data_ != nullptr)){
        delete [] data_;
        data_ = nullptr;
    }
}

void Cube::allocate(int nx, int ny, int nz){
    setSize(nx, ny, nz);
    clean();
    data_ = allocate_();
}

void Cube::copy(Cube* src) {
    allocate(src->nx(), src->ny(), src->nz());

    if ( src->type() == type_ ){
        memcpy(data_, src->data(), src->byteSize());
    }
    else {
        for( int i=0; i<size(); i++ ){
            setValueIdx(i, src->valueIdx(i));
        }
    }
}

void Cube::add(Cube* cube){
    if (( cube->nx() != nx_ ) || (cube->ny() != ny_ ) || ( cube->nz() != nz_ )){
        std::cerr << "unable to add two cube with different dimensions" << std::endl;
        return;
    }
    for( int i=0; i<size(); i++ ){
        setValueIdx(i, valueIdx(i) + cube->valueIdx(i));
    }
}

void Cube::substract(Cube* cube){
    if (( cube->nx() != nx_ ) || (cube->ny() != ny_ ) || ( cube->nz() != nz_ )){
        std::cerr << "unable to add two cube with different dimensions" << std::endl;
        return;
    }
    for( int i=0; i<size(); i++ ){
        setValueIdx(i, valueIdx(i) - cube->valueIdx(i));
    }
}


void Cube::setData(void* data){
    data_ = data;
}

/*************************************************/

CubeUC::CubeUC(bool own_memory) : Cube(UINT8, own_memory){
}

/*************************************************/

CubeI32::CubeI32(bool own_memory) : Cube(UINT32, own_memory){
}

/*************************************************/

CubeF::CubeF(bool own_memory) : Cube(FLOAT, own_memory){
}

/*************************************************/
CubeD::CubeD(bool own_memory) : Cube(DOUBLE, own_memory){
}





