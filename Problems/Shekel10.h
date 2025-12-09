//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_SHEKEL10_H
#define SF2565_PROJECT_SHEKEL10_H

#include "../Problem.h"


struct Shekel10Problem final : public Problem {
    explicit Shekel10Problem()
    : Problem(std::vector<double>(4, 0.0),
          std::vector<double>(4,  10.0)) {}

    double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        if (x.size() != 4) {
            throw std::invalid_argument("Shekel10 expects dimension 4!");
        }

        double s=0.0;
        const int d=4;

        static const double A[10][4] = {
            {4, 4, 4, 4},
            {1, 1, 1, 1},
            {8, 8, 8, 8},
            {6, 6, 6, 6},
            {3, 7, 3, 7},
            {2, 9, 2, 9},
            {5, 5, 3, 3},
            {8, 1, 8, 1},
            {6, 2, 6, 2},
            {7, 3.6, 7, 3.6}
        };

        static constexpr double c[10] = {0.1, 0.2, 0.2, 0.4, 0.4, 0.6, 0.3, 0.7, 0.5, 0.5};

        for (int i = 0; i < 10; ++i) {
            double dist2 = 0.0;
            for (int j = 0; j < d; ++j) {
                const double diff = x[j] - A[i][j];
                dist2 += diff * diff;
            }
            s += 1.0 / (dist2 + c[i]);
        }

        return -s;
    }
};


#endif //SF2565_PROJECT_SHEKEL10_H