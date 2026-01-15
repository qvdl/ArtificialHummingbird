//
// Created by quinten on 12/2/25.
//

#include "RNG_Utils.h"
#include <random>
#include <algorithm>
#include <numeric>

namespace {
    // Single RNG engine shared across the project (seeded via RNG_Utils::initialize).
    std::mt19937 engine;
}

namespace RNG_Utils {

    // Seed the global RNG engine to make runs reproducible.
    void initialize(const unsigned int seed) {
        engine.seed(seed);
    }

    // Uniform random number in [0, 1).
    // The distribution object is static to avoid re-allocating it on every call.
    double random01() {
        static std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    // Standard normal random number N(0,1).
    // Also uses a static distribution to keep per-call overhead low.
    double randomNormal() {
        static std::normal_distribution<double> dist(0.0, 1.0);
        return dist(engine);
    }

    // Returns a random permutation of {0, 1, ..., k-1}.
    std::vector<int> randomPermutation(const int k) {
        std::vector<int> indices(k);
        std::iota(indices.begin(), indices.end(), 0);
        std::ranges::shuffle(indices, engine);
        return indices;
    }

    // Uniform integer random number in [min, max] (inclusive).
    int randomInt(const int min, const int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }
}
