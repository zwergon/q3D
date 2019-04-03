
#include "tst_wave.h"

#include <cube/algo/wave1d.h>
#include <cmath>
#include <iostream>




// add necessary includes here

TestWave::TestWave()
{
}

TestWave::~TestWave()
{
}

void TestWave::test_step_even()
{

    double input[512];
    double step = 3.4 / 512.;
    double x = -1.7;
    for ( int i =0; i<512; i++ ){
        if ( x < 0 ){
            input[i] = -std::exp(-std::pow(x, 4));
        }
        else {
           input[i] = std::exp(-std::pow(x, 4));
        }
        x += step;
    }

    double output[512];
    memcpy(output, input, 512*sizeof(double));


    CDF97 cdf;
    cdf.forward = true;
    cdf.step_1D(output, 512);

    cdf.forward = false;
    cdf.step_1D(output, 512);

    double sum = 0;
    for ( int i =0; i<512; i++ ){
        //std::cout << input[i] << ", " << output[i] << std::endl;
        sum += std::pow(input[i] - output[i], 2);
    }

    QVERIFY(std::sqrt(sum) < 1e-13);
}

void TestWave::test_step_odd()
{
    double input[511];
    double step = 3.4 / 511.;
    double x = -1.7;
    for ( int i =0; i<511; i++ ){
        if ( x < 0 ){
            input[i] = -std::exp(-std::pow(x, 4));
        }
        else {
           input[i] = std::exp(-std::pow(x, 4));
        }
        x += step;
    }

    double output[511];
    memcpy(output, input, 511*sizeof(double));


    CDF97 cdf;
    cdf.forward = true;
    cdf.step_1D(output, 511);

    cdf.forward = false;
    cdf.step_1D(output, 511);

    double sum = 0;
    for ( int i =0; i<511; i++ ){
        //std::cout << input[i] << ", " << output[i] << std::endl;
        sum += std::pow(input[i] - output[i], 2);
    }

    QVERIFY(std::sqrt(sum) < 1e-13);
}

void TestWave::test_haar(){
    double input[255];

    double step = 3.14159257 / 255.;
    double x =0;
    for( int i=0; i<255; i++) {
        input[i] = 255*std::cos(x);
        x+=step;
    }

    double output[255];
    memcpy(output, input, 255*sizeof(double));


    Haar haar;
    haar.forward = true;
    haar.step_1D(output, 255);


    /*for ( int i =0; i<128; i++ ){
        std::cout << i << ", " << input[2*i] << ", " << output[i] << std::endl;
    }*/

    haar.forward = false;
    haar.step_1D(output, 255);

    double sum = 0;
    for ( int i =0; i<255; i++ ){
        //std::cout << input[i] << ", " << output[i] << std::endl;
        sum += std::pow(input[i] - output[i], 2);
    }

    QVERIFY(std::sqrt(sum) < 1e-12);

}

