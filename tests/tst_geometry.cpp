#include <QtTest>

#include <q3D/model/point3d.h>

class TestGeometry : public QObject
{
    Q_OBJECT

public:
    TestGeometry();
    ~TestGeometry();

private slots:
    void test_scalar();

};

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

QTEST_APPLESS_MAIN(TestGeometry)

#include "tst_geometry.moc"
