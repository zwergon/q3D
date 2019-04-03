#include <QtTest>
#include "tst_geometry.h"
#include "tst_wave.h"

int main(int argc, char** argv) {
    TestGeometry testGeometry;
    TestWave testWave;

    return
            QTest::qExec(&testGeometry, argc, argv) ||
            QTest::qExec(&testWave, argc, argv);
}
