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
      Cube(int type, bool own_memory = true);
      virtual void allocate_() = 0;
      virtual void free_() = 0;

      void clean();

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
class CUBESHARED_EXPORT CubeT : public Cube
{

public:
    CubeT(bool own_memory = true);

    virtual int sizeOf() const override;
    virtual double valueIdx(int idx) const override;
    virtual void setValueIdx(int idx, double value) override;

    virtual const void* data() const override;
    virtual void* data() override;
    virtual void setData(void* data) override;

protected:
    virtual void allocate_() override;
    virtual void free_() override;

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
    data_ = new T[size()];
}

template <class T>
inline void CubeT<T>::free_() {
    if ( data_ != nullptr ){
        delete [] data_;
        data_ = nullptr;
    }
}

#endif // CUBE_H
