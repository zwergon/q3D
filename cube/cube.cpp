#include "cube.h"


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

Cube* Cube::create(int type, bool own_memory){
    Cube* cube = nullptr;
    switch(type){
    case Cube::SAVEF3UC:
        cube = new CubeUC(own_memory);
        break;
    case Cube::SAVEF3I:
        cube = new CubeI32(own_memory);
        break;
    case Cube::SAVEF3F:
        cube = new CubeF(own_memory);
        break;
    case Cube::SAVEF3D:
        cube = new CubeD(own_memory);
        break;
    default:
       break;
    }

    return cube;
}

void Cube::allocate(int nx, int ny, int nz){
    setSize(nx, ny, nz);
    own_memory_ = true;
    data_ = allocate_();
}

void Cube::copyTo(Cube* dest) const{
    dest->allocate(nx_, ny_, nz_);
    for( int i=0; i<size(); i++ ){
        dest->setValueIdx(i, getValueIdx(i));
    }
}

Cube::~Cube(){
    if ( own_memory_&& (data_ != nullptr)){
        delete [] data_;
    }
}

void Cube::setData(void* data){
    data_ = data;
}

/*************************************************/

CubeUC::CubeUC(bool own_memory) : Cube(SAVEF3UC, own_memory){
}

/*************************************************/

CubeI32::CubeI32(bool own_memory) : Cube(SAVEF3I, own_memory){
}

/*************************************************/

CubeF::CubeF(bool own_memory) : Cube(SAVEF3F, own_memory){
}

/*************************************************/
CubeD::CubeD(bool own_memory) : Cube(SAVEF3D, own_memory){
}





