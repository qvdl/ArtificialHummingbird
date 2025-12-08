//
// Created by quinten on 12/2/25.
//

#include "VisitTable.h"
#include <limits>


int VisitTable::getTarget(const int birdIndex, const std::vector<Hummingbird>& population) const {
    if (population.size() != size) {
        throw std::invalid_argument("Population size does not correspond to VT size.");
    }

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

// Migration: the worst source is reinitialized uniformly in bounds.
// migratedSourceIndex: column of the source that just migrated (i.e., the worst before reinit)
void VisitTable::updateMigration(int const migratedSourceIndex) {

    // When a source is updated/replaced (here via migration),
    // every OTHER bird's memory of THAT source must jump to (row max + 1).
    // Only columns change; we do NOT treat this as a "visit", so we don't touch rows' aging/zeros.
    for (int r=0; r<size; ++r) {
        if (r==migratedSourceIndex) continue;

        int rowMax=0;
        for (int c=0; c<size; ++c) {
            if (c==r) continue;
            int v = get(r,c);
            if (v > rowMax) rowMax = v;
        }

        at(r,migratedSourceIndex) = rowMax + 1;
    }
}















