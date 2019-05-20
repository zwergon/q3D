#ifndef CUBE_H
#define CUBE_H

#include <cube/cube_global.h>

#include <vector>
#include <stdint.h>

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
    void extract(Cube* src, uint32_t idx[]);

    void add(Cube* cube);
    void substract(Cube* cube);

    int type() const;

    bool ownMemory() const;
    void setOwnMemory(bool);

    void setPixelSize( double px, double py, double pz );
    const double* pixelSize() const;

    void setOrigin( double ox, double oy, double oz );
    const double* origin() const;

    uint32_t size() const;
    long byteSize() const;
    void setSize( uint32_t nx_, uint32_t ny_, uint32_t nz_ );

    const uint32_t* dim() const;
    uint32_t nx() const;
    uint32_t ny() const;
    uint32_t nz() const;

    double value(uint32_t i, uint32_t j, uint32_t k) const;
    void setValue(uint32_t i, uint32_t j, uint32_t k, double value);

    virtual void* data() = 0;
    virtual const void* data() const = 0;
    virtual void setData(void* data) = 0;
    virtual int sizeOf() const = 0;
    virtual double valueIdx(uint32_t idx) const = 0;
    virtual void setValueIdx(uint32_t idx, double value) = 0;

    double ijk2xyz(uint32_t idx, int i) const;
    uint32_t xyz2ijk(double pos, int i) const;

    uint32_t index(uint32_t i, uint32_t j, uint32_t k) const;
    void allocate( uint32_t nx, uint32_t ny, uint32_t nz );

protected:
      Cube(bool own_memory);
      virtual void allocate_() = 0;


protected:
      bool own_memory_;
      int type_;
      uint32_t dim_[3];
      double pixel_[3];
      double origin_[3];
};

inline int Cube::type() const {
    return type_;
}

inline bool Cube::ownMemory() const {
    return own_memory_;
}

inline void Cube::setOwnMemory(bool own_memory) {
    own_memory_ = own_memory;
}

inline void Cube::setPixelSize( double px, double py, double pz ){
    pixel_[0] = px;
    pixel_[1] = py;
    pixel_[2] = pz;
}

inline const double* Cube::pixelSize() const {
    return pixel_;
}

inline void Cube::setOrigin( double ox, double oy, double oz ){
    origin_[0] = ox;
    origin_[1] = oy;
    origin_[2] = oz;
}

inline const double* Cube::origin() const{
    return origin_;
}


inline const uint32_t* Cube::dim() const {
    return dim_;
}

inline uint32_t Cube::nx() const {
    return dim_[0];
}

inline uint32_t Cube::ny() const {
    return dim_[1];
}

inline uint32_t Cube::nz() const {
    return dim_[2];
}

inline uint32_t Cube::size() const {
    return dim_[0]*dim_[1]*dim_[2];
}

inline long Cube::byteSize() const {
    return size()*sizeOf();
}

inline void Cube::setSize(uint32_t nx, uint32_t ny, uint32_t nz){
    this->dim_[0] = nx;
    this->dim_[1] = ny;
    this->dim_[2] = nz;
}

inline uint32_t Cube::index(uint32_t i, uint32_t j, uint32_t k) const {
    return k + dim_[2]*(j+i*dim_[1]);
}

inline double Cube::value(uint32_t i, uint32_t j, uint32_t k) const {
    return valueIdx(index(i, j, k));
}

inline void Cube::setValue(uint32_t i, uint32_t j, uint32_t k, double val) {
    return setValueIdx(index(i, j, k), val);
}

inline double Cube::ijk2xyz(uint32_t idx, int i) const {
    return idx*pixel_[i] + origin_[i];
}

inline uint32_t Cube::xyz2ijk( double pos, int i ) const {
    return (uint32_t)((pos - origin_[i])/pixel_[i]);
}

/*************************************************/

template <class T>
class CubeT : public Cube
{

public:
    CubeT(bool own_memory = true);
    virtual ~CubeT();

    virtual int sizeOf() const override;
    virtual double valueIdx(uint32_t idx) const override;
    virtual void setValueIdx(uint32_t idx, double value) override;

    virtual const void* data() const override;
    virtual void* data() override;
    virtual void setData(void* data) override;

protected:
    virtual void allocate_() override;
    void free_();

protected:
    T* data_;
};

template <class T>
inline int CubeT<T>::sizeOf() const {
    return sizeof(T);
}

template <class T>
inline double CubeT<T>::valueIdx(uint32_t idx) const {
    return static_cast<double>(data_[idx]);
}

template <class T>
inline void* CubeT<T>::data(){
    return data_;
}

template <class T>
inline const void* CubeT<T>::data() const{
    return data_;
}

template <class T>
void CubeT<T>::setData(void* data) {
     data_ = reinterpret_cast<T*>(data);
}

template <class T>
inline void CubeT<T>::setValueIdx(uint32_t idx, double val ) {
    data_[idx] = static_cast<T>(val);
}

template <class T>
inline void CubeT<T>::allocate_() {
    if (ownMemory()){
        free_();
    }
    data_ = new T[size()];
}

template <class T>
inline void CubeT<T>::free_() {
    if ( data_ != nullptr ){
        delete [] data_;
        data_ = nullptr;
    }
}

template <class T>
CubeT<T>::CubeT(bool own_memory )
    : Cube(own_memory),
      data_(nullptr)
{
    if ( std::is_same<T, uint8_t>::value ){
        type_ = UINT8;
    }
    else if ( std::is_same<T, uint32_t>::value ){
        type_ = UINT32;
    }
    else if ( std::is_same<T, float>::value ){
        type_ = FLOAT;
    }
    else if ( std::is_same<T, double>::value ){
        type_ = DOUBLE;
    }
}

template <class T>
CubeT<T>::~CubeT(){
    if (ownMemory()){
        free_();
    }
}

#endif // CUBE_H
