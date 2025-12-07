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

    uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    uint64_t make_seed(const uint64_t base_seed, const int iter, const int bird) {
        const uint64_t key = (static_cast<uint64_t>(static_cast<uint32_t>(iter)) << 32) ^ static_cast<uint64_t>(static_cast<uint32_t>(bird));
        return splitmix64(base_seed ^ key);
    }

    // Stream functions
    double Stream::uniform01() {
        return std::generate_canonical<double, 53>(eng);
    }
    double Stream::normal01() {
        std::normal_distribution<double> nd(0.0, 1.0);
        return nd(eng);
    }
    int Stream::randint(const int lo, const int hi) {
        std::uniform_int_distribution<int> d(lo, hi);
        return d(eng);
    }
    std::vector<int> Stream::permutation(const int n) {
        std::vector<int> p(n);
        std::iota(p.begin(), p.end(), 0);
        std::ranges::shuffle(p, eng);
        return p;
    }
}