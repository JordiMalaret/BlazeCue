#include <iostream>
#include <array>
#include "utils.h"
#include "heartRate.h"
#include <vector>
#include <deque>

std::deque<double> filteredBuffer;

std::array<double,chunkSize> chunk(){
    if(filteredBuffer.size()>=hopSize){
        filteredBuffer.erase(filteredBuffer.begin(),
                                 filteredBuffer.begin() + hopSize);
    }
    while(filteredBuffer.size() < chunkSize){
        int16_t value = getCurrentACVal();
        double filteredSample = filter(value);
        filteredBuffer.push_back(filteredSample);
    }
    // When enough samples collected, process chunk
    if (filteredBuffer.size() >= chunkSize) {
        std::array<double, chunkSize> chunk{};
        std::copy(filteredBuffer.begin(),
              filteredBuffer.begin() + chunkSize,
              chunk.begin());
        return(chunk);
    }
}
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

    static BiquadState s1 = {0.0, 0.0};
    static BiquadState s2 = {0.0, 0.0};
    
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
