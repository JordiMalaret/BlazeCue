#ifndef SPLIT_H
#define SPLIT_H

#include <array>
#include <cstdint>

constexpr std::size_t ARRAY_SIZE = 1024;
const int hopSize = 100;
const int chunkSize = 500;   // example: 10 sec at 50 Hz

std::array<int16_t, ARRAY_SIZE> split();

struct BiquadCoeffs {
    double b0, b1, b2, a1, a2;
};

struct BiquadState {
    double z1, z2;
};

double process_biquad(double x, const BiquadCoeffs& c, BiquadState& s);

double filter(int16_t x);

#endif