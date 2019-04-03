#ifndef TST_GEOMETRY_H
#define TST_GEOMETRY_H

#include <QtTest>

class TestGeometry : public QObject
{
    Q_OBJECT

public:
    TestGeometry();
    ~TestGeometry();

private slots:
    void test_scalar();

};


#endif // TST_GEOMETRY_H
