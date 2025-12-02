//
// Created by quinten on 12/2/25.
//

#include "AHA_Optimizer.h"
#include <limits>
#include "RNG_Utils.h"



AHA_Optimizer::AHA_Optimizer(const Problem &prob, const int populationSize, const int maxIter)
    : visitTable(populationSize), flightEngine(prob.getLowerBound().size()), problem(prob), maxIterations(maxIter)
{
    // initialise global best
    globalBestFitness = std::numeric_limits<double>::max();
    globalBestPosition.resize(prob.getLowerBound().size());

    // initialise population
    population.reserve(populationSize);
    for (int i=0; i<populationSize; ++i) {
        population.emplace_back(prob);
    }
}