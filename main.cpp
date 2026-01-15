#include "AHA_Optimizer.h"
#include "helpers.h"
#include "Problems/Rastrigin.h"
#include "RNG_Utils.h"
#include <chrono>
#include <iostream>

// Benchmark problem headers (all derive from Problem)
#include "Problems/AbsoluteValue.h"
#include "Problems/Ackley.h"
#include "Problems/Beale.h"
#include "Problems/Branin.h"
#include "Problems/Easom.h"
#include "Problems/Griewank.h"
#include "Problems/Michalewicz10.h"
#include "Problems/Powell.h"
#include "Problems/Rastrigin.h"
#include "Problems/Rosenbrock.h"
#include "Problems/Schwefel12.h"
#include "Problems/Schwefel226.h"
#include "Problems/Shekel10.h"
#include "Problems/ShiftedQuadratic.h"
#include "Problems/SixHumpCamelBack.h"
#include "Problems/Sphere.h"
#include "Problems/Zakharov.h"

/*
 * Simple entry point to run the Artificial Hummingbird Algorithm on a single
 * benchmark problem, as demonstration.
 */

int main() {
    // --- Parameters ---
    constexpr int population_size = 50;     // number of hummingbirds
    constexpr int nr_iterations   = 20'000; // number of AHA iterations
    constexpr int dim             = 100;    // problem dimension
    constexpr int seed            = 2026;   // base RNG seed

    // Initialize RNG once for reproducibility.
    RNG_Utils::initialize(seed + dim);

    // Select the optimization problem to solve.
    // To use a different problem, replace SphereProblem with any other
    // Problem subclass included above, for example:
    //   Schwefel226Problem prob(dim);
    //   RastriginProblem prob(dim);
    //   AckleyProblem prob(dim);
    const SphereProblem prob(dim);

    // Construct the AHA optimizer with the chosen problem and parameters.
    AHA_Optimizer aha(prob, population_size, nr_iterations);

    // Measure runtime of the solve() call.
    const auto start_time = std::chrono::high_resolution_clock::now();
    aha.solve();
    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start_time).count();

    // Report results.
    std::cout << "Solving problem took " << duration << " ms.\n";
    std::cout << "Optimal solution = " << aha.getGlobalBestFitness() << "\n";

    return 0;
}
