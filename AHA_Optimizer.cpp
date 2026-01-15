//
// Created by quinten on 12/2/25.
//

#include "AHA_Optimizer.h"
#include "RNG_Utils.h"
#include <limits>


// --- constructor ---
AHA_Optimizer::AHA_Optimizer(const Problem& prob, const int populationSize, const int maxIter)
    : visitTable(populationSize), // diagonal -1, others 0
      flightEngine(static_cast<int>(prob.getLowerBound().size())),
      globalBestPosition(prob.getLowerBound().size(), 0.0),
      globalBestFitness(std::numeric_limits<double>::infinity()),
      problem(prob),
      maxIterations(maxIter)
{
    // Reserve to avoid reallocations when constructing the population.
    population.reserve(populationSize);

    // Initialize birds uniformly within bounds and record initial global best.
    for (int i = 0; i < populationSize; ++i) {
        population.emplace_back(prob);
        if (population.back().fitness < globalBestFitness) {
            globalBestFitness = population.back().fitness;
            globalBestPosition = population.back().position;
        }
    }
}

// --- solve ---
void AHA_Optimizer::solve() {
    const int n = static_cast<int>(population.size());
    const int d = static_cast<int>(problem.getLowerBound().size());
    const int M = 2 * n;               // migration interval (paper default)
    constexpr double guidedProb = 0.5; // guided vs territorial split

    std::vector<double> v(d);          // scratch buffer for candidate solutions

    // Cache bounds by reference (used repeatedly in candidate generation and migration).
    const auto& low = problem.getLowerBound();
    const auto& upp = problem.getUpperBound();

    // Clamp candidate in-place to keep it within the feasible domain.
    auto clamp_inplace = [&](std::vector<double>& w) noexcept {
        for (int k = 0; k < d; ++k) {
            if (w[k] < low[k])      w[k] = low[k];
            else if (w[k] > upp[k]) w[k] = upp[k];
        }
    };

    // Select one flight pattern uniformly and return its mask D (0/1 per dimension).
    auto pickFlightMask = [&]() -> std::vector<int> {
        const double u = RNG_Utils::random01();
        if (u < 1.0 / 3.0) return flightEngine.axialFlight();
        if (u < 2.0 / 3.0) return flightEngine.diagonalFlight();
        return flightEngine.omniFlight();
    };

    // Guided foraging candidate (uses target source chosen from visit table).
    auto guidedCandidateEval = [&](const std::vector<double>& xi,
                                   const std::vector<double>& xj) -> double {
        const auto D = pickFlightMask();
        const double a = RNG_Utils::randomNormal();
        for (int k = 0; k < d; ++k) {
            const double step = D[k] ? (xi[k] - xj[k]) : 0.0;
            v[k] = std::fma(a, step, xj[k]); // candidate coordinate
        }
        clamp_inplace(v);
        return problem.evaluate(v);
    };

    // Territorial foraging candidate (local perturbation around current source).
    auto territorialCandidateEval = [&](const std::vector<double>& xi) -> double {
        const auto D = pickFlightMask();
        const double b = RNG_Utils::randomNormal();
        for (int k = 0; k < d; ++k) {
            const double step = D[k] ? xi[k] : 0.0;
            v[k] = std::fma(b, step, xi[k]);
        }
        clamp_inplace(v);
        return problem.evaluate(v);
    };

    // Main optimization loop.
    for (int iter = 1; iter <= maxIterations; ++iter) {
        for (int i = 0; i < n; ++i) {
            bool replaced = false; // tracks whether greedy acceptance happened

            // Choose foraging mode (guided only makes sense when n >= 2).
            if (RNG_Utils::random01() < guidedProb && n >= 2) {
                const int j = visitTable.getTarget(i, population); // target source index
                auto&       bird = population[i];
                const auto& tar  = population[j];

                const double fv = guidedCandidateEval(bird.position, tar.position);
                if (fv < bird.fitness) { // greedy acceptance
                    bird.position = v;   // copy from scratch buffer once
                    bird.fitness  = fv;
                    replaced = true;

                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = v;
                    }
                }

                visitTable.updateGuided(i, j, replaced);

            } else {
                auto& bird = population[i];

                const double fv = territorialCandidateEval(bird.position);
                if (fv < bird.fitness) {
                    bird.position = v;
                    bird.fitness  = fv;
                    replaced = true;

                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = v;
                    }
                }

                visitTable.updateTerritorial(i, replaced);
            }
        }

        // Migration: every M iterations, replace the worst source with a random one.
        if (iter % M == 0) {
            int worst = 0;
            double worstFit = population[0].fitness;
            for (int i = 1; i < n; ++i) {
                if (population[i].fitness > worstFit) {
                    worst = i;
                    worstFit = population[i].fitness;
                }
            }

            // Reinitialize worst position uniformly within bounds.
            for (int k = 0; k < d; ++k) {
                const double r = RNG_Utils::random01();
                population[worst].position[k] = low[k] + r * (upp[k] - low[k]);
            }
            population[worst].fitness = problem.evaluate(population[worst].position);

            visitTable.updateMigration(worst);

            if (population[worst].fitness < globalBestFitness) {
                globalBestFitness = population[worst].fitness;
                globalBestPosition = population[worst].position;
            }
        }
    }
}
