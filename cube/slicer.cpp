#include "slicer.h"

Slicer2D::Slicer2D(const Cube& cube, int cursor) :
    cube_(cube),
    cursor_(cursor)
{
}

Slicer2D::~Slicer2D(){
}

int XSlicer2D::getNx() const {
    return cube_.ny();
}

int XSlicer2D::getNy() const {
    return cube_.nz();
}

double XSlicer2D::getValue(int i, int j) const {
    return cube_.value(cursor_, i, j);
}


YSlicer2D::YSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}

int YSlicer2D::getNx() const {
    return cube_.nx();
}

int YSlicer2D::getNy() const {
    return cube_.nz();
}

double YSlicer2D::getValue(int i, int j) const {
    return cube_.value(i, cursor_, j);
}

ZSlicer2D::ZSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}

int ZSlicer2D::getNx() const {
    return cube_.nx();
}

int ZSlicer2D::getNy() const {
    return cube_.ny();
}

double ZSlicer2D::getValue(int i, int j) const {
    return cube_.value(i, j, cursor_);
}

XSlicer2D::XSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}

