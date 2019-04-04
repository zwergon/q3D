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
    void extract(Cube* src, int idx[]);

    void add(Cube* cube);
    void substract(Cube* cube);

    int type() const;

    bool ownMemory() const;
    void setOwnMemory(bool);

    int size() const;
    long byteSize() const;
    void setSize( int nx_, int ny_, int nz_ );

    int nx() const;
    int ny() const;
    int nz() const;

    double value(int i, int j, int k) const;
    void setValue(int i, int j, int k, double value);

    virtual void* data() = 0;
    virtual const void* data() const = 0;
    virtual void setData(void* data) = 0;
    virtual int sizeOf() const = 0;
    virtual double valueIdx(int idx) const = 0;
    virtual void setValueIdx(int idx, double value) = 0;

    int index(int i, int j, int k) const;
    void allocate( int nx, int ny, int nz );

protected:
      Cube(bool own_memory);
      virtual void allocate_() = 0;


protected:
      bool own_memory_;
      int type_;
      int nx_;
      int ny_;
      int nz_;

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

/*************************************************/

template <class T>
class CubeT : public Cube
{

public:
    CubeT(bool own_memory = true);
    virtual ~CubeT();

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

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
inline double CubeT<T>::valueIdx(int idx) const {
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
inline void CubeT<T>::setValueIdx(int idx, double val ) {
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
