#ifndef CUBE_H
#define CUBE_H

#include <cube/cube_global.h>

#include <vector>

/*************************************************/

class CUBESHARED_EXPORT Cube
{
public:
    enum DATA_TYPE {
        UINT8 = 300,
        UINT32,
        FLOAT,
        DOUBLE
    };

    static Cube* create(int type, bool own_memory = true);

public:
    virtual ~Cube();

    void copy(Cube* src);
    void extract(Cube* src, int idx[]);

    void add(Cube* cube);
    void substract(Cube* cube);

    int type() const;

    int size() const;
    long byteSize() const;
    void setSize( int nx_, int ny_, int nz_ );

    int nx() const;
    int ny() const;
    int nz() const;

    void* data();
    const void* data() const;
    void setData(void* data);

    double value(int i, int j, int k) const;
    void setValue(int i, int j, int k, double value);

    virtual int sizeOf() const = 0;
    virtual double valueIdx(int idx) const = 0;
    virtual void setValueIdx(int idx, double value) = 0;

    int index(int i, int j, int k) const;
    void allocate( int nx, int ny, int nz );

protected:
      Cube(int type, bool own_memory = true);
      virtual void* allocate_() = 0;

      void clean();

protected:
      bool own_memory_;
      int type_;
      int nx_;
      int ny_;
      int nz_;
      void* data_;
};

inline int Cube::type() const {
    return type_;
}

inline int Cube::nx() const {
    return nx_;
}

inline int Cube::ny() const {
    return ny_;
}

inline int Cube::nz() const {
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

inline double Cube::value(int i, int j, int k) const {
    return valueIdx(index(i, j, k));
}

inline void Cube::setValue(int i, int j, int k, double val) {
    return setValueIdx(index(i, j, k), val);
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
    CubeUC(bool own_memory = true);

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

    const uint8_t* data() const;
    uint8_t* data();

protected:
    virtual void* allocate_() override;
};

inline int CubeUC::sizeOf() const {
    return sizeof(uint8_t);
}

inline double CubeUC::valueIdx(int idx) const {
    return data()[idx];
}

inline const uint8_t* CubeUC::data() const{
    return reinterpret_cast<uint8_t*>(data_);
}

inline void CubeUC::setValueIdx(int idx, double val ) {
    data()[idx] = (uint8_t)val;
}

inline uint8_t* CubeUC::data(){
    return reinterpret_cast<uint8_t*>(data_);
}

inline void* CubeUC::allocate_() {
    return new uint8_t[size()];
}

/*************************************************/

class CUBESHARED_EXPORT CubeI32 : public Cube
{

public:
    CubeI32(bool own_memory = true);

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

    const int32_t* data() const;
    int32_t* data();

protected:
    virtual void* allocate_() override;
};

inline int CubeI32::sizeOf() const {
    return sizeof(int32_t);
}

inline double CubeI32::valueIdx(int idx) const {
    return data()[idx];
}

inline void CubeI32::setValueIdx(int idx, double val ) {
    data()[idx] = (int32_t)val;
}

inline const int32_t* CubeI32::data() const{
    return reinterpret_cast<int32_t*>(data_);
}

inline int32_t* CubeI32::data(){
    return reinterpret_cast<int32_t*>(data_);
}

inline void* CubeI32::allocate_() {
    return new int32_t[size()];
}

/*************************************************/

class CUBESHARED_EXPORT CubeF : public Cube
{

public:
    CubeF(bool own_memory = true);

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

    const float *data() const;
    float* data();


protected:
    virtual void* allocate_() override;
};

inline int CubeF::sizeOf() const {
    return sizeof(float);
}

inline double CubeF::valueIdx(int idx) const {
    return data()[idx];
}

inline void CubeF::setValueIdx(int idx, double val ) {
    data()[idx] = (float)val;
}

inline const float* CubeF::data() const{
    return reinterpret_cast<float*>(data_);
}

inline float* CubeF::data(){
    return reinterpret_cast<float*>(data_);
}

inline void* CubeF::allocate_() {
    return new float[size()];
}


/*************************************************/

class CUBESHARED_EXPORT CubeD : public Cube
{

public:
    CubeD(bool own_memory = true);

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

    const double* data() const;
    double* data();


protected:
    virtual void* allocate_() override;
};

inline int CubeD::sizeOf() const {
    return sizeof(double);
}

inline double CubeD::valueIdx(int idx) const {
    return data()[idx];
}

inline void CubeD::setValueIdx(int idx, double val ) {
    data()[idx] = val;
}

inline const double* CubeD::data() const{
    return reinterpret_cast<double*>(data_);
}

inline double* CubeD::data(){
    return reinterpret_cast<double*>(data_);
}

inline void* CubeD::allocate_() {
    return new double[size()];
}


#endif // CUBE_H
