#include "AHA_Optimizer.h"
#include "helpers.h"
#include "Problems/Rastrigin.h"
#include "RNG_Utils.h"
#include <chrono>
#include <iostream>


int main() {
    // Experiment knobs
    constexpr int population_size = 50;
    constexpr int nr_iterations   = 20'000;
    constexpr int dim             = 30;
    constexpr int seed            = 2025;

    RNG_Utils::initialize(seed);
    const RastriginProblem rastrigin(dim);
    AHA_Optimizer aha(rastrigin, population_size, nr_iterations);

    const auto start_time = std::chrono::high_resolution_clock::now();
    // aha.solve();
    aha.solve_optimized();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count();

    std::cout << "Solving Rastrigin problem took " << duration << " ms.\n";
    std::cout << "Optimal solution = " << aha.getGlobalBestFitness() << "\n";

    return 0;
}
