#include "geometry.h"

namespace Q3D {


#define EPSILON 1e-8

/**
 * @brief Geometry::__rayIntersectsTriangle Möller–Trumbore intersection algorithm
 *
 * @param t return t double value so that impart = ray.origin + t * ray.vector
 *
 */
bool Geometry::rayIntersectsTriangle(
        const Point3d& rayOrigin,
        const Point3d& rayVector,
        const Point3d& vertex0, const Point3d& vertex1, const Point3d& vertex2,
        double& t)
{

    Point3d edge1, edge2, h, s, q;
    float a,f,u,v;
    for( int i=0; i<3; i++){
        edge1[i] = vertex1[i] - vertex0[i];
        edge2[i] = vertex2[i] - vertex0[i];
    }
    h = Q3D::cross<double>(rayVector, edge2);
    a = Q3D::scalar<double>(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.

    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * Q3D::scalar<double>(s, h);
    if (u < 0.0 || u > 1.0)
        return false;

    q = Q3D::cross<double>(s, edge1);
    v = f * Q3D::scalar<double>(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    t = f * Q3D::scalar<double>(edge2, q);
    return t > EPSILON; // ray intersection

}

}
