//
// Created by quinten on 12/2/25.
//

#include "VisitTable.h"
#include <limits>

/*
 * Target selection for guided foraging.
 * We pick the source j that bird i has not visited for the longest time (max VT[i,j]).
 * If multiple sources have the same visit time, we break ties by choosing the one with
 * the best (lowest) fitness in the current population.
 */
int VisitTable::getTarget(const int birdIndex, const std::vector<Hummingbird>& population) const {
    if (population.size() != size) {
        throw std::invalid_argument("Population size does not correspond to VT size.");
    }

    int bestTarget = -1;
    int maxVisitTime = -1;
    double bestFitness = std::numeric_limits<double>::max();

    for (int j = 0; j < size; ++j) {
        if (j == birdIndex) continue; // diagonal is not a valid target

        const int visitTime = get(birdIndex, j);

        if (visitTime > maxVisitTime) {
            maxVisitTime = visitTime;
            bestTarget = j;
            bestFitness = population[j].getFitness();
        } else if (visitTime == maxVisitTime) {
            const double candidateFitness = population[j].getFitness();
            if (candidateFitness < bestFitness) {
                bestTarget = j;
                bestFitness = candidateFitness;
            }
        }
    }

    return bestTarget;
}

/*
 * Visit-table update after guided foraging.
 * - Age all non-visited entries in row 'currentBirdIndex' (except diagonal and chosen target).
 * - Mark the chosen target as "just visited" by setting VT[i,target] = 0.
 * - If the target source was replaced (greedy acceptance), update the corresponding column
 *   to reflect that this source is "new" to all other birds.
 */
void VisitTable::updateGuided(const int currentBirdIndex, const int targetBirdIndex, const bool sourceReplaced) {
    for (int c = 0; c < size; ++c) {
        if (c == currentBirdIndex || c == targetBirdIndex) continue;
        int& value = at(currentBirdIndex, c);
        if (value != -1) value += 1; // keep diagonal untouched
    }

    at(currentBirdIndex, targetBirdIndex) = 0;
    at(currentBirdIndex, currentBirdIndex) = -1;

    if (sourceReplaced) {
        for (int r = 0; r < size; ++r) {
            if (r == currentBirdIndex) continue;

            int rowMax = 0;
            for (int c = 0; c < size; ++c) {
                if (c == r) continue; // skip diagonal
                const int v = get(r, c);
                if (v > rowMax) rowMax = v;
            }

            // Column update: set VT[r, currentBirdIndex] to (max row value + 1).
            at(r, currentBirdIndex) = rowMax + 1;
        }
    }
}

/*
 * Visit-table update after territorial foraging (bird explores around its own source).
 * - Age all entries in the resident bird's row, except the diagonal.
 * - If the resident's source was replaced, broadcast a column update to other birds
 *   (same rule as in guided update).
 */
void VisitTable::updateTerritorial(int const residentBirdIndex, bool const sourceReplaced) {
    for (int c = 0; c < size; ++c) {
        if (c == residentBirdIndex) continue;
        int& v = at(residentBirdIndex, c);
        if (v != -1) v += 1;
    }

    if (sourceReplaced) {
        for (int r = 0; r < size; ++r) {
            if (r == residentBirdIndex) continue;

            int rowMax = 0;
            for (int c = 0; c < size; ++c) {
                if (c == r) continue;
                const int v = get(r, c);
                if (v > rowMax) rowMax = v;
            }

            at(r, residentBirdIndex) = rowMax + 1;
        }
    }
}

/*
 * Visit-table update after migration.
 * Migration replaces a source but is not treated as an explicit "visit" event.
 * Therefore we only update the affected column (for all other birds) and do not
 * reset or age any rows.
 */
void VisitTable::updateMigration(int const migratedSourceIndex) {
    for (int r = 0; r < size; ++r) {
        if (r == migratedSourceIndex) continue;

        int rowMax = 0;
        for (int c = 0; c < size; ++c) {
            if (c == r) continue;
            const int v = get(r, c);
            if (v > rowMax) rowMax = v;
        }

        at(r, migratedSourceIndex) = rowMax + 1;
    }
}
