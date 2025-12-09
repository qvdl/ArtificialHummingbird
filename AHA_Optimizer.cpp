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
    population.reserve(populationSize);
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

    // AHA migration coefficient (paper default): migrate every 2n iters.
    const int M = 2 * n;

    // AHA uses 50/50 split between Guided and Territorial foraging.
    constexpr double guidedProb = 0.5;

    // Reusable candidate scratch
    std::vector<double> v(d);

    // Cache bounds once (used in candidates and migration)
    const auto& low = problem.getLowerBound();
    const auto& upp = problem.getUpperBound();

    // In-place clamp helper (no extra vector copies)
    auto clamp_inplace = [&](std::vector<double>& w) noexcept {
        for (int k = 0; k < d; ++k) {
            if (w[k] < low[k])      w[k] = low[k];
            else if (w[k] > upp[k]) w[k] = upp[k];
        }
    };

    // Pick one of the three flight skills with equal probability (Axial/Diagonal/Omni).
    // Matches the flight masks D in the paper (Fig. 3; Eqs. (3)–(5)).
    auto pickFlightMask = [&]() -> std::vector<int> {
        const double u = RNG_Utils::random01();
        if (u < 1.0/3.0) return flightEngine.axialFlight();
        if (u < 2.0/3.0) return flightEngine.diagonalFlight();
        return flightEngine.omniFlight();
    };

    // Build guided candidate into 'v' and return its fitness.
    // v = x_j + a * D * (x_i - x_j), a ~ N(0,1)
    auto guidedCandidateEval = [&](const std::vector<double>& xi,
                                   const std::vector<double>& xj) -> double {
        const auto D = pickFlightMask();
        const double a = RNG_Utils::randomNormal();  // Eq.(7)
        for (int k = 0; k < d; ++k) {
            const double step = D[k] ? (xi[k] - xj[k]) : 0.0; // Eqs.(3)-(6)
            v[k] = std::fma(a, step, xj[k]);
        }
        clamp_inplace(v);                             // keep domain valid
        return problem.evaluate(v);
    };

    // Build territorial candidate into 'v' and return its fitness.
    // v = x_i + b * D * x_i, b ~ N(0,1)
    auto territorialCandidateEval = [&](const std::vector<double>& xi) -> double {
        const auto D = pickFlightMask();
        const double b = RNG_Utils::randomNormal();  // Eq.(10)
        for (int k = 0; k < d; ++k) {
            const double step = D[k] ? xi[k] : 0.0;  // Eqs.(3)-(5), (9)
            v[k] = std::fma(b, step, xi[k]);
        }
        clamp_inplace(v);
        return problem.evaluate(v);
    };

    // main optimization loop
    for (int iter = 1; iter <= maxIterations; ++iter) {
        // --- per-bird foraging phase (each iteration does either Guided or Territorial) ---
        for (int i = 0; i < n; ++i) {
            bool replaced = false;

            // With probability 0.5 (and n >= 2), do GUIDED foraging; otherwise TERRITORIAL.
            if (RNG_Utils::random01() < guidedProb && n >= 2) {
                // --- GUIDED FORAGING ---
                const int j = visitTable.getTarget(i, population);
                auto&       bird = population[i];
                const auto& tar  = population[j];

                const double fv = guidedCandidateEval(bird.position, tar.position);
                if (fv < bird.fitness) {
                    bird.position = v;            // copy once from the scratch
                    bird.fitness  = fv;
                    replaced = true;
                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = v;
                    }
                }

                // Visit-table maintenance (guided)
                visitTable.updateGuided(i, j, replaced);

            } else {
                // --- TERRITORIAL FORAGING ---
                auto& bird2 = population[i];

                const double fv2 = territorialCandidateEval(bird2.position);
                if (fv2 < bird2.fitness) {
                    bird2.position = v;
                    bird2.fitness  = fv2;
                    replaced = true;
                    if (fv2 < globalBestFitness) {
                        globalBestFitness = fv2;
                        globalBestPosition = v;
                    }
                }

                // Visit-table maintenance (territorial)
                visitTable.updateTerritorial(i, replaced);
            }
        }

        // --- migration every M = 2n iterations ---
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

            // reinitialize worst uniformly in bounds (reuse cached low/upp)
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


void AHA_Optimizer::solve_optimized() {
    const int n = static_cast<int>(population.size());
    const int d = static_cast<int>(problem.getLowerBound().size());

    // AHA migration coefficient (paper default): migrate every 2n iters.
    const int M = 2 * n;

    // AHA uses 50/50 split between Guided and Territorial foraging.
    constexpr double guidedProb = 0.5;

    // Reusable candidate scratch
    std::vector<double> v(d);

    // Cache bounds once (used in candidates and migration)
    const auto& low = problem.getLowerBound();
    const auto& upp = problem.getUpperBound();

    // Pick one of the three flight skills with equal probability (Axial/Diagonal/Omni).
    // Matches the flight masks D in the paper (Fig. 3; Eqs. (3)–(5)).
    auto pickFlightMask = [&]() -> std::vector<int> {
        const double u = RNG_Utils::random01();
        if (u < 1.0/3.0) return flightEngine.axialFlight();
        if (u < 2.0/3.0) return flightEngine.diagonalFlight();
        return flightEngine.omniFlight();
    };

    // --- keep your existing code above ---

    // Build guided candidate into 'v' and return its fitness.
    // v = x_j + a * D * (x_i - x_j), a ~ N(0,1)
    auto guidedCandidateEval = [&](const std::vector<double>& xi,
                                   const std::vector<double>& xj) -> double {
        const auto D = pickFlightMask();
        const double a = RNG_Utils::randomNormal();  // Eq.(7)
        for (int k = 0; k < d; ++k) {
            // branchless mask: cast 0/1 to double and multiply
            const auto m = static_cast<double>(D[k]);
            // v[k] = xj[k] + a * (xi[k] - xj[k]) * m;
            // use FMA for throughput where available
            v[k] = std::fma(a * m, (xi[k] - xj[k]), xj[k]);
        }
        // clamp in a way that vectorizes well
        for (int k = 0; k < d; ++k) {
            v[k] = std::clamp(v[k], low[k], upp[k]);
        }
        return problem.evaluate(v);
    };

    // Build territorial candidate into 'v' and return its fitness.
    // v = x_i + b * D * x_i, b ~ N(0,1)
    auto territorialCandidateEval = [&](const std::vector<double>& xi) -> double {
        const auto D = pickFlightMask();
        const double b = RNG_Utils::randomNormal();  // Eq.(10)
        for (int k = 0; k < d; ++k) {
            const auto m = static_cast<double>(D[k]);
            // v[k] = xi[k] + b * xi[k] * m;
            v[k] = std::fma(b * m, xi[k], xi[k]);
        }
        for (int k = 0; k < d; ++k) {
            v[k] = std::clamp(v[k], low[k], upp[k]);
        }
        return problem.evaluate(v);
    };


    // main optimization loop
    for (int iter = 1; iter <= maxIterations; ++iter) {
        // --- per-bird foraging phase (each iteration does either Guided or Territorial) ---
        for (int i = 0; i < n; ++i) {
            bool replaced = false;

            // With probability 0.5 (and n >= 2), do GUIDED foraging; otherwise TERRITORIAL.
            if (RNG_Utils::random01() < guidedProb && n >= 2) {
                // --- GUIDED FORAGING ---
                const int j = visitTable.getTarget(i, population);
                auto&       bird = population[i];
                const auto& tar  = population[j];

                const double fv = guidedCandidateEval(bird.position, tar.position);
                if (fv < bird.fitness) {
                    bird.position = v;            // copy once from the scratch
                    bird.fitness  = fv;
                    replaced = true;
                    if (fv < globalBestFitness) {
                        globalBestFitness = fv;
                        globalBestPosition = v;
                    }
                }

                // Visit-table maintenance (guided)
                visitTable.updateGuided(i, j, replaced);

            } else {
                // --- TERRITORIAL FORAGING ---
                auto& bird2 = population[i];

                const double fv2 = territorialCandidateEval(bird2.position);
                if (fv2 < bird2.fitness) {
                    bird2.position = v;
                    bird2.fitness  = fv2;
                    replaced = true;
                    if (fv2 < globalBestFitness) {
                        globalBestFitness = fv2;
                        globalBestPosition = v;
                    }
                }

                // Visit-table maintenance (territorial)
                visitTable.updateTerritorial(i, replaced);
            }
        }

        // --- migration every M = 2n iterations ---
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

            // reinitialize worst uniformly in bounds (reuse cached low/upp)
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
