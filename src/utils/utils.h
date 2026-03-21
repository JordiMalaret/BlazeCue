#ifndef SPLIT_H
#define SPLIT_H

#include <array>
#include <cstdint>

constexpr std::size_t ARRAY_SIZE = 1024;

std::array<int16_t, ARRAY_SIZE> split();

#endif