// RNG_Utils.h
#pragma once
#include <random>
#include <vector>

namespace RNG_Utils {
    void initialize(unsigned int seed);
    double random01();
    double randomNormal();
    std::vector<int> randomPermutation(int k);
    int randomInt(int min, int max);
} // namespace RNG_Utils