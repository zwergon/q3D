#ifndef CUBE_H
#define CUBE_H

#include "cube_global.h"

#include <QSharedMemory>
#include <QDebug>

#include <vector>

namespace Q3D {

class CUBESHARED_EXPORT Cube
{

public:
    Cube();
    virtual ~Cube();

    int size() const;
    void setSize( int nx_, int ny_, int nz_ );
    int getNx() const;
    int getNy() const;
    int getNz() const;

    void setData( quint8* );
    const quint8* data() const;

    void attach( QSharedMemory& sharedMemory );

    quint8 getValue( int i, int j, int k) const;
    int index(int i, int j, int k) const;

private:
    int nx_;
    int ny_;
    int nz_;
    QSharedMemory data_;
};

inline int Cube::getNx() const {
    return nx_;
}

inline int Cube::getNy() const {
    return ny_;
}

inline int Cube::getNz() const {
    return nz_;
}

inline int Cube::size() const {
    return nx_*ny_*nz_;
}

inline void Cube::setSize(int nx, int ny, int nz){
    this->nx_ = nx;
    this->ny_ = ny;
    this->nz_ = nz;
}

inline const quint8* Cube::data() const{
    return (quint8*)data_.data();
}

inline int Cube::index(int i, int j, int k) const {
    return i + (k+j*nz_)*ny_;
}

inline quint8 Cube::getValue(int i, int j, int k) const {
    quint8 val = data()[index(i,j,k)];
    return  val;
}


}
#endif // CUBE_H
