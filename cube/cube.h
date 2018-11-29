#ifndef CUBE_H
#define CUBE_H

#include "cube_global.h"

namespace Q3D {


class CUBESHARED_EXPORT Cube
{

public:
    Cube();

    int getNx() const;
    int getNy() const;
    int getNz() const;
    int index(int i, int j, int k) const;
    quint8 getValue( int i, int j, int k) const;

    void setSize( int nx, int ny, int nz );
    void setData( quint8* );

    quint8* data() const;

private:
    int nx;
    int ny;
    int nz;
    quint8* data_;
};

inline int Cube::getNx() const {
    return nx;
}

inline int Cube::getNy() const {
    return ny;
}

inline int Cube::getNz() const {
    return nz;
}

inline void Cube::setSize(int nx, int ny, int nz){
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

}

inline quint8* Cube::data() const{
    return data_;
}

inline void Cube::setData(quint8* data){
    data_ = data;
}

inline int Cube::index(int i, int j, int k) const {
    return i + (k+j*nz)*ny;
}

inline quint8 Cube::getValue(int i, int j, int k) const {
    return data_[index(i,j,k)];
}


}
#endif // CUBE_H
