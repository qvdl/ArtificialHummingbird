//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_RNG_UTILS_H
#define SF2565_PROJECT_RNG_UTILS_H

#include <vector>

namespace RNG_Utils {
    void initialize(unsigned int seed);
    double random01();
    double randomNormal();
    std::vector<int> randomPermutation(int k);
    int randomInt(int min, int max);
} // RNG_Utils

#endif //SF2565_PROJECT_RNG_UTILS_H