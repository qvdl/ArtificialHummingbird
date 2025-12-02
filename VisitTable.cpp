//
// Created by quinten on 12/2/25.
//

#include "VisitTable.h"
#include <limits>


int VisitTable::getTarget(const int birdIndex, const std::vector<Hummingbird>& population) const {
    int bestTarget=-1;
    int maxVisitTime=-1;
    double bestFitness = std::numeric_limits<double>::max();

    for (int j=0; j<size; ++j) {
        if (j==birdIndex) continue;

        int visitTime = get(birdIndex, j);

        // found new maximum
        if (visitTime > maxVisitTime) {
            maxVisitTime = visitTime;
            bestTarget = j;
            bestFitness = population[j].getFitness();
        }

        // tie: compare fitness
        else if (visitTime == maxVisitTime) {
            double candidateFitness = population[j].getFitness();
            if (candidateFitness < bestFitness) {
                bestTarget = j;
                bestFitness = candidateFitness;
            }
        }
    }

    return bestTarget;
}

void VisitTable::updateGuided(const int currentBirdIndex, const int targetBirdIndex, const bool foodChanged) {
    // increment all food sources in currentBird's row
    for (int j=0; j<size; ++j) {
        if (j == currentBirdIndex) continue;
        int& value = at(currentBirdIndex, j);
        if (value != -1) { value += 1; }
    }

    // reset currentBird's visit to targetBird
    at(currentBirdIndex, targetBirdIndex) = 0;

    // if food improved, update column
    if (foodChanged) {
        // find maximum in targetBird's column
        int maxVal = -1;
        for (int i=0; i<size; ++i) {
            if (i==targetBirdIndex) continue;
            int val = get(i,targetBirdIndex);
            if (val > maxVal) { maxVal = val; }
        }

        // update entries in targetBird's column
        for (int i=0; i<size; ++i) {
            if (i == currentBirdIndex || i == targetBirdIndex) { continue; }
            at(i, targetBirdIndex) = maxVal+1;
        }
    }
}
