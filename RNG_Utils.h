// RNG_Utils.h
#pragma once
#include <random>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace RNG_Utils {
    void initialize(unsigned int seed);
    double random01();
    double randomNormal();
    std::vector<int> randomPermutation(int k);
    int randomInt(int min, int max);

    // --- deterministic seeding helper for parallel use ---
    uint64_t splitmix64(uint64_t x);
    uint64_t make_seed(uint64_t base_seed, int iter, int bird);

    // --- per-task RNG stream (thread-safe by construction) ---
    struct Stream {
        explicit Stream(const uint64_t seed) : eng(seed) {}

        double uniform01();
        double normal01();
        int randint(int lo, int hi);
        std::vector<int> permutation(int n);
    private:
        std::mt19937_64 eng;
    };
} // namespace RNG_Utils