#ifndef Q3D_MODEL_TYPES_H
#define Q3D_MODEL_TYPES_H

#include <q3D/dlib/matrix.h>
#include <q3D/wykobi/wykobi.hpp>

namespace Q3D {

typedef dlib::matrix<double,0,1> vector;

typedef wykobi::point3d<double> Point3d;
typedef wykobi::point3d<float> Point3f;
typedef wykobi::box<double,3> Box3d;

}

#endif // Q3D_MODEL_TYPES_H
