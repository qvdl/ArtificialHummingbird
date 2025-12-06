//
// Created by quinten on 12/6/25.
//

#ifndef SF2565_PROJECT_RASTRIGINPROBLEM_H
#define SF2565_PROJECT_RASTRIGINPROBLEM_H


#include <vector>
#include <cmath>
#include "../Problem.h"


struct RastriginProblem : public Problem {
    explicit RastriginProblem(const int dimension)
        : Problem(std::vector<double>(dimension, -5.12),
            std::vector<double>(dimension, 5.12)) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        constexpr double A = 10.0;
        constexpr double two_pi = 2.0 * M_PI;
        const int d = static_cast<int>(x.size());
        double s = A*d;
        for (int i=0; i<d; ++i) {
            const double xi = x[i];
            s += xi*xi - A*std::cos(two_pi * xi);
        }
        return s;
    }
};



#endif //SF2565_PROJECT_RASTRIGINPROBLEM_H