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
    if (dimension <= 1) {                    // degenerate; fall back to omni on 1-D
        std::fill(mask.begin(), mask.end(), 1);
        return mask;
    }

    // Paper's k range: 2 .. ceil(r1*(d-2)) + 1, clamped into [2, d]
    const double r1 = RNG_Utils::random01();
    int kmax = static_cast<int>(std::ceil(r1 * (dimension - 2))) + 1;
    if (kmax < 2) kmax = 2;
    if (kmax > dimension) kmax = dimension;

    // If RNG_Utils::randomInt(a,b) is inclusive, this is fine.
    // If it's half-open [a,b), change to randomInt(2, kmax+1) - 1.
    const int k = RNG_Utils::randomInt(2, kmax);

    // Pick k unique axes with partial Fisher–Yates (O(k)).
    std::vector<int> idx(dimension);
    std::iota(idx.begin(), idx.end(), 0);
    for (int t = 0; t < k; ++t) {
        const int r = t + RNG_Utils::randomInt(0, dimension - 1 - t);
        std::swap(idx[t], idx[r]);
        mask[idx[t]] = 1;
    }
    return mask;
}

// equation (5)
std::vector<int> FlightEngine::omniFlight() const {
    return std::vector<int>(dimension, 1);
}
