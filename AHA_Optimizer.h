//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_AHA_OPTIMIZER_H
#define SF2565_PROJECT_AHA_OPTIMIZER_H

#include <vector>
#include "Hummingbird.h"
#include "VisitTable.h"
#include "Problem.h"
#include "FlightEngine.h"


/*
 * AHA_Optimizer orchestrates the Artificial Hummingbird Algorithm:
 *   - Initializes a population of Hummingbird objects (solutions)
 *   - Runs the main AHA loop (guided / territorial foraging + periodic migration)
 *   - Tracks the global best solution found so far
 */

class AHA_Optimizer {
private:
    std::vector<Hummingbird> population;
    VisitTable visitTable;
    FlightEngine flightEngine;

    std::vector<double> globalBestPosition;
    double globalBestFitness;

    const Problem& problem;
    int maxIterations;

public:
    AHA_Optimizer(const Problem& prob, int populationSize, int maxIter);

    void solve(); // main algorithm
    void solve_optimized();

    [[nodiscard]] const std::vector<double>& getGlobalBestPosition() const { return globalBestPosition; }
    [[nodiscard]] double getGlobalBestFitness() const { return globalBestFitness; }
};


#endif //SF2565_PROJECT_AHA_OPTIMIZER_H