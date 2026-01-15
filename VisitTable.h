#ifndef SF2565_PROJECT_VISITTABLE_H
#define SF2565_PROJECT_VISITTABLE_H

#include <vector>
#include <stdexcept>
#include "Hummingbird.h"

/*
 * VisitTable (VT) stores "time since last visit" information used by AHA.
 *
 * Interpretation:
 *   - Row    = hummingbird index i
 *   - Column = food source index j (also corresponds to a hummingbird's current source)
 *   - VT(i,j) = how many iterations ago bird i visited source j
 *              (0 means "just visited" this iteration)
 *
 * Convention:
 *   - Diagonal VT(i,i) is invalid and stored as -1 ("null" in the paper).
 *
 * Implementation note:
 *   The table is stored as a flat 1D vector for cache-friendly access.
 */

class VisitTable {
private:
    std::vector<int> matrix; // flattened N x N table
    size_t size;             // N (population size)

    // Convert (row, col) to a flat index. Throws on out-of-range access.
    [[nodiscard]] int getIndex(const int row, const int col) const {
        if (row < 0 || col < 0 || row >= size || col >= size) {
            throw std::out_of_range("VisitTable index out of range!");
        }
        return row * size + col;
    }

public:
    // Initialize VT with zeros, and set diagonal entries to -1 ("null").
    explicit VisitTable(const int N) : matrix(N * N, 0), size(N) {
        for (int k = 0; k < size; ++k) {
            matrix[getIndex(k, k)] = -1; // -1 represents "null" in equation (2)
        }
    }

    // Read-only access to VT(i,j).
    [[nodiscard]] int get(const int row, const int col) const {
        return matrix[getIndex(row, col)];
    }

    // Mutable access to VT(i,j) when updates are needed.
    int& at(const int row, const int col) { return matrix[getIndex(row, col)]; }

    // Convenience increment used for aging entries (diagonal is excluded by design).
    void increment(const int row, const int col) {
        if (row != col) matrix[getIndex(row, col)] += 1;
    }

    // Select a target source for guided foraging based on VT values (ties broken by fitness).
    [[nodiscard]] int getTarget(int birdIndex, const std::vector<Hummingbird>& population) const;

    [[nodiscard]] size_t getSize() const { return size; }

    // Update rules for VT after each foraging mode / migration event.
    void updateGuided(int currentBirdIndex, int targetBirdIndex, bool sourceReplaced);
    void updateTerritorial(int residentBirdIndex, bool sourceReplaced);
    void updateMigration(int migratedSourceIndex);
};

#endif //SF2565_PROJECT_VISITTABLE_H
