
#include "tst_geometry.h"

#include <q3D/model/point3d.h>

TestGeometry::TestGeometry()
{
}

TestGeometry::~TestGeometry()
{
}

void TestGeometry::test_scalar()
{
    Q3D::Point3d pt1 = {1, 2, 3};
    Q3D::Point3d pt2 = {1, 2, 3};
    double s = Q3D::scalar<double>(pt1, pt2);
    QCOMPARE(s, 14.);
}
