#include "cube.h"


namespace Q3D {


/*************************************************/
Cube::Cube() :
    nx_(0),
    ny_(0),
    nz_(0),
    data_()
{
}

Cube::~Cube(){
    if (data_.isAttached()){
        data_.detach();
    }
}

void Cube::setData(void* data){
    data_.create(byteSize());
    data_.lock();
    memcpy(data_.data(), data, data_.size());
    data_.unlock();
}

void Cube::attach(QSharedMemory &sharedMemory){
    data_.setKey(sharedMemory.key());
    data_.attach();
    qDebug() << "create cube " << sharedMemory.key();
}

/*************************************************/

CubeUC::CubeUC() : Cube(){
}

/*************************************************/

CubeF::CubeF() : Cube(){
}


}



