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

void VisitTable::updateGuided(const int currentBirdIndex, const int targetBirdIndex, const bool sourceReplaced) {
    // increment all others, set target to 0 (and diagonal -1)
    for (int c=0; c<size; ++c) {
        if (c == currentBirdIndex || c == targetBirdIndex) continue;
        int& value = at(currentBirdIndex, c);
        if (value != -1) { value += 1; }
    }

    at(currentBirdIndex, targetBirdIndex) = 0;
    at(currentBirdIndex, currentBirdIndex) = -1;

    // if food improved, update column
    if (sourceReplaced) {
        for (int r=0; r<size; ++r) {
            if (r == currentBirdIndex) continue;

            int rowMax=0;
            for (int c=0; c<size; ++c) {
                if (c == r) continue; // skip diagonal
                int v = get(r,c);
                if (v>rowMax) rowMax=v;
            }

            // update the column of the replaced source
            at(r,currentBirdIndex) = rowMax + 1;
        }
    }
}

// Territorial foraging: the bird perturbs its own neighborhood (target == itself).
// residentBirdIndex: the hummingbird whose own source/territory was just explored
// sourceReplaced: true iff the greedy check accepted the territorial candidate and
// the bird now "resides" at a new source (its column changed)
void VisitTable::updateTerritorial(int const residentBirdIndex, bool const sourceReplaced) {
    // age all non-visited sources for this bird (everything except its own source).
    for (int c=0; c<size; ++c) {
        if (c==residentBirdIndex) continue;
        int &v = at(residentBirdIndex, c);
        if (v != -1) v+=1;
    }

    // mark own source as "just visited"
    at(residentBirdIndex, residentBirdIndex)=0;


    // if the bird's own source was replaced by the territorial candidate,
    // broadcast that update to everyone else:
    // For every other bird r, set VT[r, i] = (max over row r, excluding diag) + 1
    if (sourceReplaced) {
        for (int r=0; r<size; ++r) {
            if (r==residentBirdIndex) continue;

            int rowMax=0;
            for (int c=0; c<size; ++c) {
                if (c==r) continue; // skip diagonal
                int v = get(r,c);
                if (v > rowMax) rowMax = v;
            }

            at(r, residentBirdIndex) = rowMax+1; // update column of the replaced source
        }
    }
}
















