#ifndef CUBE_H
#define CUBE_H

#include "cube_global.h"

#include <QSharedMemory>
#include <QDebug>

#include <vector>

namespace Q3D {

/*************************************************/

class CUBESHARED_EXPORT Cube
{

public:
    Cube();
    virtual ~Cube();

    int size() const;
    long byteSize() const;
    void setSize( int nx_, int ny_, int nz_ );
    int getNx() const;
    int getNy() const;
    int getNz() const;

    double getValue(int i, int j, int k) const;
    virtual int sizeOf() const = 0;
    virtual double getValue(int idx) const = 0;

    void setData(void* data);
    void attach( QSharedMemory& sharedMemory );

    int index(int i, int j, int k) const;

protected:
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

inline long Cube::byteSize() const {
    return size()*sizeOf();
}

inline void Cube::setSize(int nx, int ny, int nz){
    this->nx_ = nx;
    this->ny_ = ny;
    this->nz_ = nz;
}

inline int Cube::index(int i, int j, int k) const {
    return k + j*nz_ + i*nz_*ny_;
}

inline double Cube::getValue(int i, int j, int k) const {
    return getValue(index(i, j, k));
}

/*************************************************/

class CUBESHARED_EXPORT CubeUC : public Cube
{

public:
    CubeUC();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const quint8* data() const;
};

inline int CubeUC::sizeOf() const {
    return sizeof(quint8);
}



inline double CubeUC::getValue(int idx) const {
    return data()[idx];
}

inline const quint8* CubeUC::data() const{
    return (quint8*)data_.data();
}

/*************************************************/

class CUBESHARED_EXPORT CubeF : public Cube
{

public:
    CubeF();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const float* data() const;
};

inline int CubeF::sizeOf() const {
    return sizeof(float);
}

inline double CubeF::getValue(int idx) const {
    return data()[idx];
}

inline const float* CubeF::data() const{
    return (float*)data_.data();
}


}
#endif // CUBE_H
