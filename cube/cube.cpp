#include "cube.h"


/*************************************************/
Cube::Cube(int type) :
    type_(type),
    nx_(0),
    ny_(0),
    nz_(0),
    data_()
{
}

Cube* Cube::create(int type){
    Cube* cube = nullptr;
    switch(type){
    case Cube::SAVEF3UC:
        cube = new CubeUC;
        break;
    case Cube::SAVEF3I:
        cube = new CubeI32;
        break;
    case Cube::SAVEF3F:
        cube = new CubeF;
        break;
    case Cube::SAVEF3D:
        cube = new CubeD;
        break;
    default:
       break;
    }

    return cube;
}

Cube::~Cube(){
    if (data_ != nullptr){
        delete [] data_;
    }
}

void Cube::setData(void* data){
    data_ = data;
}

/*************************************************/

CubeUC::CubeUC() : Cube(SAVEF3UC){
}

/*************************************************/

CubeI32::CubeI32() : Cube(SAVEF3I){
}

/*************************************************/

CubeF::CubeF() : Cube(SAVEF3F){
}

/*************************************************/
CubeD::CubeD() : Cube(SAVEF3D){
}





