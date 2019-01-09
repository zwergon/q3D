#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <q3D/model/model_scope.h>

#include <q3D/model/point3d.h>

namespace Q3D {

class MODELSHARED_EXPORT Geometry
{
public:
    static bool rayIntersectsTriangle(
            const Point3d& rayOrigin,
            const Point3d& rayVector,
            const Point3d& vertex0, const Point3d& vertex1, const Point3d& vertex2,
            double& t);
};

}

#endif // GEOMETRY_H
