#ifndef Q3D_MODEL_POINT3D_H
#define Q3D_MODEL_POINT3D_H

#include <q3D/model/model_scope.h>

#include <array>
#include <cmath>
#include <limits>

namespace Q3D {


inline bool AreSame(double a, double b) {
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

typedef std::array<double, 3> Point3d;
typedef std::array<Point3d, 2> Box3d;

template<class T> std::array<T,3> max_positif_point3() {
        return { std::numeric_limits<T>::max(),
        std::numeric_limits<T>::max(),
        std::numeric_limits<T>::max() };
    };

template<class T> std::array<T,3> min_negatif_point3() {
    return {
        std::numeric_limits<T>::lowest(),
        std::numeric_limits<T>::lowest(),
        std::numeric_limits<T>::lowest() };
};

template<class T> std::array<T, 3> cross( const std::array<T,3>& v1, const std::array<T,3>& v2 )
{
    return {
        v1[1]*v2[2] - v1[2]*v2[1],
        v1[2]*v2[0] - v1[0]*v2[2],
        v1[0]*v2[1] - v1[1]*v2[0]
    };
}

template<class T> T scalar(const std::array<T,3>& v1, const std::array<T, 3>& v2){
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

template<class T> void normalize(std::array<T,3>& v)
{
    T r = sqrt( scalar<T>(v, v) );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

}

#endif // Q3D_MODEL_POINT3D_H
