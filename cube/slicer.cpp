#include "slicer.h"

namespace Q3D {

Slicer2D::Slicer2D(const Cube& cube, int cursor) :
    cube_(cube),
    cursor_(cursor)
{
}

Slicer2D::~Slicer2D(){
}

int XSlicer2D::getNx() const {
    return cube_.getNy();
}

int XSlicer2D::getNy() const {
    return cube_.getNz();
}

quint8 XSlicer2D::getValue(int i, int j) const {
    return cube_.getValue(cursor_, i, j);
}


YSlicer2D::YSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}

int YSlicer2D::getNx() const {
    return cube_.getNx();
}

int YSlicer2D::getNy() const {
    return cube_.getNz();
}

quint8 YSlicer2D::getValue(int i, int j) const {
    return cube_.getValue(i, cursor_, j);
}

ZSlicer2D::ZSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}

int ZSlicer2D::getNx() const {
    return cube_.getNx();
}

int ZSlicer2D::getNy() const {
    return cube_.getNy();
}

quint8 ZSlicer2D::getValue(int i, int j) const {
    return cube_.getValue(i, j, cursor_);
}

XSlicer2D::XSlicer2D(const Cube& cube, int cursor) : Slicer2D(cube, cursor){
}



}
