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

    bool transform( bool forward );


private:
    void forward_transform();
    void reverse_transform();

private:
    Cube* cube_;
    std::unique_ptr<Wave1D> wave_;
};

#endif // WAVE97_H
