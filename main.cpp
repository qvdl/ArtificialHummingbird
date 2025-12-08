#include "helpers.h"
#include "Problems/Rosenbrock.h"
#include "RNG_Utils.h"


int main() {
    constexpr int population_size = 100;
    constexpr int nr_iterations = 10'000;
    constexpr int dimension = 10;
    constexpr int seed = 1234;

    sanity_tests(population_size, nr_iterations, dimension, seed);

    return 0;
}