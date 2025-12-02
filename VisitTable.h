//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_VISITTABLE_H
#define SF2565_PROJECT_VISITTABLE_H

#include <vector>
#include <stdexcept>


/*
 * row: hummingbird
 * column: food source
 * value at (i,j): how many iterations ago did bird i visit food source j (0 means just visited)
 */

class VisitTable {
private:
    std::vector<int> matrix;
    int size;

    [[nodiscard]] int getIndex(const int row, const int col) const {
        if (row<0 || col<0 || row>=size || col>=size) { throw std::out_of_range("VisitTable index out of range!"); }
        return row * size + col;
    }

public:
    explicit VisitTable(const int N) : matrix(N*N,0), size(N) {
        for (int k=0; k<size; ++k) {
            matrix[ getIndex(k,k) ] = -1; // -1 represents "null" in equation (2)
        }
    }

    [[nodiscard]] int get(const int row, const int col) const {
        return matrix[getIndex(row, col)];
    }

    // getTarget()
    // updateGuided()
    // updateMigration()
};


#endif //SF2565_PROJECT_VISITTABLE_H