#ifndef CUBE_H
#define CUBE_H

#include <cube/cube_global.h>

#include <vector>

/*************************************************/

class CUBESHARED_EXPORT Cube
{
public:
    enum DATA_TYPE {
        SAVEF3UC = 300,
        SAVEF3I,
        SAVEF3F,
        SAVEF3D
    };

    static Cube* create(int type);

public:

    virtual ~Cube();

    int type() const;

    int size() const;
    long byteSize() const;

    void setSize( int nx_, int ny_, int nz_ );
    int getNx() const;
    int getNy() const;
    int getNz() const;

    void* data();
    const void* data() const;

    double getValue(int i, int j, int k) const;
    virtual int sizeOf() const = 0;
    virtual double getValue(int idx) const = 0;

    void setData(void* data);

    int index(int i, int j, int k) const;

protected:
      Cube(int type);

protected:
    int type_;
    int nx_;
    int ny_;
    int nz_;
    void* data_;
};

inline int Cube::type() const {
    return type_;
}

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

inline const void* Cube::data() const{
    return data_;
}

inline void* Cube::data() {
    return data_;
}

/*************************************************/

class CUBESHARED_EXPORT CubeUC : public Cube
{

public:
    CubeUC();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const uint8_t* data() const;
    uint8_t* data();
};

inline int CubeUC::sizeOf() const {
    return sizeof(uint8_t);
}

inline double CubeUC::getValue(int idx) const {
    return data()[idx];
}

inline const uint8_t* CubeUC::data() const{
    return reinterpret_cast<uint8_t*>(data_);
}


inline uint8_t* CubeUC::data(){
    return reinterpret_cast<uint8_t*>(data_);
}

/*************************************************/

class CUBESHARED_EXPORT CubeI32 : public Cube
{

public:
    CubeI32();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const int32_t* data() const;
    int32_t* data();
};

inline int CubeI32::sizeOf() const {
    return sizeof(int32_t);
}

inline double CubeI32::getValue(int idx) const {
    return data()[idx];
}

inline const int32_t* CubeI32::data() const{
    return reinterpret_cast<int32_t*>(data_);
}

inline int32_t* CubeI32::data(){
    return reinterpret_cast<int32_t*>(data_);
}

/*************************************************/

class CUBESHARED_EXPORT CubeF : public Cube
{

public:
    CubeF();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const float *data() const;
    float* data();
};

inline int CubeF::sizeOf() const {
    return sizeof(float);
}

inline double CubeF::getValue(int idx) const {
    return data()[idx];
}

inline const float* CubeF::data() const{
    return reinterpret_cast<float*>(data_);
}

inline float* CubeF::data(){
    return reinterpret_cast<float*>(data_);
}

/*************************************************/

class CUBESHARED_EXPORT CubeD : public Cube
{

public:
    CubeD();

    virtual int sizeOf() const override;
    virtual double getValue(int idx) const override;

    const double* data() const;
    double* data();
};

inline int CubeD::sizeOf() const {
    return sizeof(double);
}

inline double CubeD::getValue(int idx) const {
    return data()[idx];
}

inline const double* CubeD::data() const{
    return reinterpret_cast<double*>(data_);
}

inline double* CubeD::data(){
    return reinterpret_cast<double*>(data_);
}

#endif // CUBE_H
