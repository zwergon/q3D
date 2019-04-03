#ifndef TST_WAVE_H
#define TST_WAVE_H

#include <QtTest>

class TestWave : public QObject
{
    Q_OBJECT

public:
    TestWave();
    ~TestWave();

private slots:
    void test_step_even();
    void test_step_odd();
    void test_haar();

};

#endif // TST_WAVE_H
