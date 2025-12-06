//
// Created by quinten on 12/2/25.
//


#include "AHA_Optimizer.h"
#include "RNG_Utils.h"
#include <limits>
#include <cmath>
#include <algorithm>


// ---- constructor ------------------------------------------------------------
AHA_Optimizer::AHA_Optimizer(const Problem& prob, int populationSize, int maxIter)
    : population(),               // filled below
      visitTable(populationSize), // diagonal -1, others 0
      flightEngine(static_cast<int>(prob.getLowerBound().size())),
      globalBestPosition(prob.getLowerBound().size(), 0.0),
      globalBestFitness(std::numeric_limits<double>::infinity()),
      problem(prob),
      maxIterations(maxIter)
{
    // Initialize population uniformly in bounds
    population.reserve(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        population.emplace_back(prob);              // your Hummingbird(prob) ctor
        if (population.back().fitness < globalBestFitness) {
            globalBestFitness = population.back().fitness;
            globalBestPosition = population.back().position;
        }
    }
}

// ---- solve ------------------------------------------------------------------
void AHA_Optimizer::solve() {
    const int n = static_cast<int>(population.size());
    const int d = static_cast<int>(problem.getLowerBound().size());
    const int M = 2 * n;                 // migration coefficient (paper default)
    constexpr double guidedProb = 0.5;       // 50/50 guided vs territorial

    auto pickFlightMask = [&]() -> std::vector<int> {
        // choose axial / diagonal / omni with equal probability
        const double u = RNG_Utils::random01();
        if (u < 1.0/3.0)      return flightEngine.axialFlight();
        if (u < 2.0/3.0) return flightEngine.diagonalFlight();
        return flightEngine.omniFlight();
    };

    auto guidedCandidate = [&](const std::vector<double>& xi,
                               const std::vector<double>& xj) -> std::vector<double> {
        const auto D = pickFlightMask();
        const double a = RNG_Utils::randomNormal();          // N(0,1)
        std::vector<double> v(d);
        for (int k = 0; k < d; ++k) {
            const double diff = xi[k] - xj[k];
            v[k] = xj[k] + a * (D[k] ? diff : 0.0);
        }
        return problem.clipToBoundaries(v);
    };

    auto territorialCandidate = [&](const std::vector<double>& xi) -> std::vector<double> {
        const auto D = pickFlightMask();
        const double b = RNG_Utils::randomNormal();          // N(0,1)
        std::vector<double> v(d);
        for (int k = 0; k < d; ++k) {
            v[k] = xi[k] + b * (D[k] ? xi[k] : 0.0);
        }
        return problem.clipToBoundaries(v);
    };

    for (int iter = 1; iter <= maxIterations; ++iter) {

        // --- per-bird foraging ---
        for (int i = 0; i < n; ++i) {
            bool replaced = false;

            // Do guided foraging with prob. 0.5, if population is at least 2
            if (RNG_Utils::random01() < guidedProb && n >= 2) {
                // ------- GUIDED FORAGING -------
                const int j = visitTable.getTarget(i, population); // target by visit-level, tie-break by fitness

                // build candidate near target
                const auto v   = guidedCandidate(population[i].position, population[j].position);
                const double fv = problem.evaluate(v);

                // greedy acceptance (minimization)
                if (fv < population[i].fitness) {
                    population[i].position = v;
                    population[i].fitness  = fv;
                    replaced = true;

                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = population[i].position;
                    }
                }

                // visit-table maintenance
                visitTable.updateGuided(i, j, replaced);

            } else {
                // ------- TERRITORIAL FORAGING -------
                const auto v   = territorialCandidate(population[i].position);
                const double fv = problem.evaluate(v);

                if (fv < population[i].fitness) {
                    population[i].position = v;
                    population[i].fitness  = fv;
                    replaced = true;

                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = population[i].position;
                    }
                }

                // visit-table maintenance
                visitTable.updateTerritorial(i, replaced);
            }
        }

        // --- migration every M iterations ------------------------------------
        if (iter % M == 0) {
            // find worst by fitness (highest, since we minimize)
            int worst = 0;
            double worstFit = population[0].fitness;
            for (int i = 1; i < n; ++i) {
                if (population[i].fitness > worstFit) {
                    worst = i;
                    worstFit = population[i].fitness;
                }
            }

            // reinitialize worst uniformly in bounds
            const auto& low = problem.getLowerBound();
            const auto& upp = problem.getUpperBound();

            population[worst].position.resize(d);
            for (int k = 0; k < d; ++k) {
                const double r = RNG_Utils::random01();
                population[worst].position[k] = low[k] + r * (upp[k] - low[k]);
            }
            population[worst].fitness = problem.evaluate(population[worst].position);

            // visit-table maintenance for replacement of column 'worst'
            visitTable.updateMigration(worst);

            // update global best if needed
            if (population[worst].fitness < globalBestFitness) {
                globalBestFitness = population[worst].fitness;
                globalBestPosition = population[worst].position;
            }
        }
    }
}
