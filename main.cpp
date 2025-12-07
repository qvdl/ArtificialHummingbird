#include "helpers.h"

int main() {
    constexpr int population_size = 50;
    constexpr int nr_iterations = 1000;
    constexpr int dimension = 30;
    constexpr int seed = 123;

    sanity_tests(population_size, nr_iterations, dimension, seed);

    return 0;
}