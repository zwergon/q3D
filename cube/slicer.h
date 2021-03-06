#ifndef SLICER_H
#define SLICER_H

#include <cube/cube_global.h>
#include <cube/cube.h>


class CUBESHARED_EXPORT Slicer2D {
public:
    Slicer2D(const Cube& cube, int cursor);
    virtual ~Slicer2D();

    virtual int getNx() const = 0;
    virtual int getNy() const = 0;
    virtual double getValue( int i, int j ) const = 0;

protected:
    const Cube& cube_;
    int cursor_;
};

class CUBESHARED_EXPORT XSlicer2D : public Slicer2D {
public:
    XSlicer2D(const Cube& cube, int cursor);

    virtual int getNx() const;
    virtual int getNy() const;
    virtual double getValue( int i, int j ) const;
};

class CUBESHARED_EXPORT YSlicer2D : public Slicer2D {
public:
    YSlicer2D(const Cube& cube, int cursor);

    virtual int getNx() const;
    virtual int getNy() const;
    virtual double getValue( int i, int j ) const;
};

class CUBESHARED_EXPORT ZSlicer2D : public Slicer2D {
public:
    ZSlicer2D(const Cube& cube, int cursor);

    virtual int getNx() const;
    virtual int getNy() const;
    virtual double getValue( int i, int j ) const;
};

#endif // SLICER_H
