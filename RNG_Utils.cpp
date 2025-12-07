//
// Created by quinten on 12/2/25.
//

#include "RNG_Utils.h"
#include <random>
#include <algorithm>
#include <numeric>

namespace {
    std::mt19937 engine;
}


namespace RNG_Utils {
    void initialize(const unsigned int seed) {
        engine.seed(seed);
    }

    double random01() {
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    double randomNormal() {
        static std::normal_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    std::vector<int> randomPermutation(const int k) {
        std::vector<int> indices(k);
        std::iota(indices.begin(), indices.end(), 0);
        std::ranges::shuffle(indices, engine);
        return indices;
    }

    int randomInt(const int min, const int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }
}