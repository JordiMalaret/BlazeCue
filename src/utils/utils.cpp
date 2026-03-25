#include <iostream>
#include <array>
#include "utils.h"
#include "heartRate.h"
#include <vector>
#include <deque>


double process_biquad(double x, const BiquadCoeffs& c, BiquadState& s) {
    double y = c.b0 * x + s.z1;
    s.z1 = c.b1 * x - c.a1 * y + s.z2;
    s.z2 = c.b2 * x - c.a2 * y;
    return y;
}

double filter(int16_t x) {
    BiquadCoeffs sec1 = {
        0.036574835844,
        0.073149671688,
        0.036574835844,
       -1.420350820932,
        0.583359123024
    };

    BiquadCoeffs sec2 = {
        1.000000000000,
       -2.000000000000,
        1.000000000000,
       -1.916260921524,
        0.920864358847
    };

    BiquadState s1 = {0.0, 0.0};
    BiquadState s2 = {0.0, 0.0};
    
    double y = process_biquad(x, sec1, s1);
    y = process_biquad(y, sec2, s2);

    return y;
}

std::array<int16_t, ARRAY_SIZE> split(){
    std::array<int16_t, ARRAY_SIZE> dataArr;
    int count = 0;
    while(count<1024){
        int16_t data = getCurrentACVal();
        dataArr[count] = data;
        count++;
    }
    return(dataArr);
}
