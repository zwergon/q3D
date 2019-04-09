#ifndef WAVE97_H
#define WAVE97_H


#include <cube/cube_global.h>

#include <cube/cube.h>
#include <cube/algo/wave1d.h>

#include <memory>

class CUBESHARED_EXPORT Wavelet {
public:
    typedef enum WaveletType_ {
        HAAR_TYPE,
        DB4_TYPE,
        CDF97_TYPE
    } WaveletType;

public:
    Wavelet(Cube* cube, int type);
    virtual ~Wavelet();

    bool transform( bool forward );

    Cube* approximation() const;

private:
    void forward_transform();
    void reverse_transform();

private:
    Cube* cube_;
    Wave1D* wave_;
};

#endif // WAVE97_H
