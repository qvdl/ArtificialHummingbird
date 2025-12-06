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

    // in 1D, diagonal move stays on the only axis
    if (dimension <= 1) {
        if (dimension == 1) { mask[0] = 1; }
        return mask;
    }
    // compute random r1 in (0, 1]
    double r1 = RNG_Utils::random01();
    if (r1 <= 0.0) r1 = std::nextafter(0.0, 1.0);

    // compute random k
    const int k_min = 2;
    int k_max = static_cast<int>(std::ceil(r1*(dimension-2)) + 1);
    if (k_max < k_min) k_max=k_min;
    if (k_max > dimension) k_max=dimension;

    const int k = RNG_Utils::randomInt(k_min, k_max);

    // sample k unique axes from [0..dimension-1]
    const auto fullPermutation = RNG_Utils::randomPermutation(dimension);
    for (int idx=0; idx<k; ++idx) {
        mask[ fullPermutation[idx] ] = 1;
    }

    return mask;
}

// equation (5)
std::vector<int> FlightEngine::omniFlight() const {
    return std::vector<int>(dimension, 1);
}
