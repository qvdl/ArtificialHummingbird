//
// Created by quinten on 12/2/25.
//

#include "FlightEngine.h"
#include "RNG_Utils.h"
#include <cmath>


// equation (3)
std::vector<int> FlightEngine::axialFlight() const {
    std::vector<int> mask(dimension, 0);
    mask[ RNG_Utils::randomInt(0, dimension-1)] = 1;
    return mask;
}

// equation (4)
std::vector<int> FlightEngine::diagonalFlight() const {
    std::vector<int> mask(dimension, 0);
    const double r1 = RNG_Utils::random01(); // r1 in (0, 1]
    const int k_min = 2;
    const int k_max = static_cast<int>(std::ceil(r1*(dimension-2)) + 1);
    const int k = RNG_Utils::randomInt(k_min, k_max);

    const auto permutation = RNG_Utils::randomPermutation(k);
    for (int i=0; i<k; ++i) {
        mask[permutation[i]] = 1;
    }

    return mask;
}

// equation (5)
std::vector<int> FlightEngine::omniFlight() const {
    return std::vector<int>(dimension, 1);
}
