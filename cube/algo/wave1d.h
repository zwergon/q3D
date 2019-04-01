#ifndef WAVE1D_H
#define WAVE1D_H

#include <cube/cube_global.h>


class CUBESHARED_EXPORT Wave1D
{
public:
    bool forward;

    void step_1D(double* input, int length) const;

protected:
    virtual void _step_1D(double* input, int length) const = 0;
    virtual void _inv_step_1D(double* input, int length) const = 0;
};


class CUBESHARED_EXPORT CDF97 : public Wave1D
{
protected:
    virtual void _step_1D(double* input, int length) const override;
    virtual void _inv_step_1D(double* input, int length) const override;
};


class CUBESHARED_EXPORT DB4 : public Wave1D {

public:
     void half_reconstruct(double* input, int length ) const;

private:
    static double hc[4];
    static double gc[4];
    static double Ihc[4];
    static double Igc[4];

private:
    virtual void _step_1D(double* input, int length) const override;
    virtual void _inv_step_1D(double* input, int length) const override;
};

class CUBESHARED_EXPORT Haar : public Wave1D
{
public:
    Haar(){}
protected:
    virtual void _step_1D(double* input, int length) const override;
    virtual void _inv_step_1D(double* input, int length) const override;
};

#endif // WAVE1D_H
