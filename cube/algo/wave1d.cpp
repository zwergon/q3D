#include "wave1d.h"

#include <vector>
#include <iostream>


/*********************************************/

void Wave1D::step_1D(double* input, int length) const{
    if (forward){
        _step_1D(input, length);
    }
    else {
        _inv_step_1D(input, length);
    }
}

/*********************************************/

// Lifting filter coefficients
static const double lfc[4] = {-1.5861343420693648, -0.0529801185718856, 0.8829110755411875, 0.4435068520511142};

// Scale factor
static const double scl = 1.1496043988602418;

static const double ext[3] =
{
    -2*lfc[0]*lfc[1]*lfc[2]/(1+2*lfc[1]*lfc[2]),
    -2*lfc[1]*lfc[2]/(1+2*lfc[1]*lfc[2]),
    -2*(lfc[0]+lfc[2]+3*lfc[0]*lfc[1]*lfc[2])/(1+2*lfc[1]*lfc[2])
};

void CDF97::_step_1D(double* input, int length) const {
    double a;
    int i;

    bool is_odd = length%2 == 1;

    // increase by 1 if length is odd
    int w_len = length + ( (is_odd) ? 1 : 0 );

    std::vector<double> work(w_len);
    memcpy(work.data(), input, length*sizeof(double));

    if ( is_odd ){
        //fill the added value with an extension that gives zero as last coefficient
        work[w_len-1] = work[w_len-4]*ext[0] + work[w_len-3]*ext[1] + work[w_len-2]*ext[2];
    }

    // Predict 1
    a=lfc[0];
    for (i=1;i<w_len-2;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[w_len-1]+=2*a*work[w_len-2];

    // Update 1
    a=lfc[1];
    for (i=2;i<w_len;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[0]+=2*a*work[1];

    // Predict 2
    a=lfc[2];
    for (i=1;i<w_len-2;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[w_len-1]+=2*a*work[w_len-2];

    // Update 2
    a=lfc[3];
    for (i=2;i<w_len;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[0]+=2*a*work[1];

    // Scale && Pack

    int offset = w_len/2;
    int o_off = length/2;

    for (i=0;i<o_off;i++)
    {
        input[i]        = work[2*i]*scl;
        input[i+offset] = work[2*i+1]/scl;
    }

    if (is_odd){
        input[o_off] = work[w_len-2]*scl;
    }

    //std::cout << offset << ", " << o_off << ", " << w_len-2 << std::endl;
}

void CDF97::_inv_step_1D(double* input, int length) const{
    double a;
    int i;

    bool is_odd = length%2 == 1;

    // increase by 1 if length is odd
    int w_len = length + ( (is_odd) ? 1 : 0 );

    // Unpack && Unscale
    std::vector<double> work(w_len);
    for (i=0;i<length/2;i++)
    {
        work[i*2]  = input[i]/scl;
        work[i*2+1]= input[i+w_len/2]*scl;
    }

    if (is_odd){
        work[w_len-2] = input[length/2]/scl;
        work[w_len-1] = 0;
    }

    // Undo update 2
    a=-lfc[3];
    for (i=2;i<w_len;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[0]+=2*a*work[1];

    // Undo predict 2
    a=-lfc[2];
    for (i=1;i<w_len-2;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[w_len-1]+=2*a*work[w_len-2];

    // Undo update 1
    a=-lfc[1];
    for (i=2;i<w_len;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[0]+=2*a*work[1];

    // Undo predict 1
    a=-lfc[0];
    for (i=1;i<w_len-2;i+=2)
    {
        work[i]+=a*(work[i-1]+work[i+1]);
    }
    work[w_len-1]+=2*a*work[w_len-2];

    memcpy(input, work.data(), length*sizeof(double));
}

static double sqrt_3 = 1.7320508075688772935274463415059;
static double denom = 5.6568542494923801952067548968388;

double DB4::hc[] = {
    (1+sqrt_3)/denom,
    (3+sqrt_3)/denom,
    (3-sqrt_3)/denom,
    (1-sqrt_3)/denom
};

double DB4::gc[] = {
    hc[3],
    -hc[2],
    hc[1],
    -hc[0]
};

double DB4::Ihc[] = {
    hc[2],
    gc[2],
    hc[0],
    gc[0]
};

double DB4::Igc[] = {
    hc[3],
    gc[3],
    hc[1],
    gc[1]
};



void DB4::_step_1D(double* input, int length) const {
    if (length >= 4) {
        int i, j;
        int half = length >> 1;

        std::vector<double> work(length);

        i = 0;
        for (j = 0; j < length-3; j = j + 2) {
            work[i]      = input[j]*hc[0] + input[j+1]*hc[1] + input[j+2]*hc[2] + input[j+3]*hc[3];
            work[i+half] = input[j]*gc[0] + input[j+1]*gc[1] + input[j+2]*gc[2] + input[j+3]*gc[3];
            i++;
        }

        work[i]      = input[length-2]*hc[0] + input[length-1]*hc[1] + input[0]*hc[2] + input[1]*hc[3];
        work[i+half] = input[length-2]*gc[0] + input[length-1]*gc[1] + input[0]*gc[2] + input[1]*gc[3];

        memcpy(input, work.data(), length*sizeof(double));
    }
}

void DB4::half_reconstruct(double* input, int length) const {
    if (length >= 4) {
        int i, j;
        std::vector<double> work(length);

        work[0] = .5*(input[length-1]*Ihc[0]  + input[0]*Ihc[2] + input[length-1]*Igc[0] + input[0]*Igc[2]);
        for (i = 0; i < length-1; i++) {
            work[i+1] = .5*(input[i]*Ihc[0] + input[i+1]*Ihc[2] + input[i]*Igc[0] + input[i+1]*Igc[2]);
        }

        memcpy(input, work.data(), length*sizeof(double));
    }
}


void DB4::_inv_step_1D(double* input, int length) const {
    if (length >= 4) {
        int i, j;
        int half = length >> 1;
        int halfPls1 = half + 1;

         std::vector<double> work(length);

        work[0] = input[half-1]*Ihc[0] + input[length-1]*Ihc[1] + input[0]*Ihc[2] + input[half]*Ihc[3];
        work[1] = input[half-1]*Igc[0] + input[length-1]*Igc[1] + input[0]*Igc[2] + input[half]*Igc[3];
        j = 2;
        for (i = 0; i < half-1; i++) {
            work[j++] = input[i]*Ihc[0] + input[i+half]*Ihc[1] + input[i+1]*Ihc[2] + input[i+halfPls1]*Ihc[3];
            work[j++] = input[i]*Igc[0] + input[i+half]*Igc[1] + input[i+1]*Igc[2] + input[i+halfPls1]*Igc[3];
        }

        memcpy(input, work.data(), length*sizeof(double));
    }
}


void Haar::_step_1D(double* input, int length) const {

    bool is_odd = length%2 == 1;

    // increase by 1 if length is odd
    int w_len = length + ( (is_odd) ? 1 : 0 );

    int half = w_len >> 1;

    // Unpack && Unscale
    std::vector<double> work(w_len);

    for (int i = 0; i<half; i++) {
         work[i]      = .5*(input[2*i] + input[2*i+1]);
         work[i+half] = .5*(input[2*i] - input[2*i+1]);
    }

    if ( is_odd ){
        work[half-1] = input[length-1];
    }

    memcpy(input, work.data(), length*sizeof(double));

}

void Haar::_inv_step_1D(double* input, int length) const {
    bool is_odd = length%2 == 1;

    // increase by 1 if length is odd
    int w_len = length + ( (is_odd) ? 1 : 0 );

    int half = w_len >> 1;

    // Unpack && Unscale
    std::vector<double> work(w_len);
    for (int i = 0; i < half; i++) {
        work[2*i]   = input[i] + input[i+half];
        work[2*i+1] = input[i] - input[i+half];
    }

    if ( is_odd ){
        work[length-1] = input[half-1];
    }

    memcpy(input, work.data(), length*sizeof(double));
}
