//
// Created by quinten on 12/6/25.
//

#ifndef SF2565_PROJECT_RASTRIGINPROBLEM_H
#define SF2565_PROJECT_RASTRIGINPROBLEM_H


#include <vector>
#include <cmath>
#include "../Problem.h"


// MULTIMODAL -- SEPERABLE
// default dimesnion = 30
// solution f=0 at x=0

struct RastriginProblem final : public Problem {
    explicit RastriginProblem(const int dimension=30)
        : Problem(std::vector<double>(dimension, -5.12),
            std::vector<double>(dimension, 5.12)) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        constexpr double A = 10.0;
        constexpr double two_pi = 2.0 * M_PI;
        const int d = static_cast<int>(x.size());
        double s = 0.0;
        for (int i=0; i<d; ++i) {
            const double xi = x[i];
            s += xi*xi - A*std::cos(two_pi * xi) + A;
        }
        return s;
    }
};



#endif //SF2565_PROJECT_RASTRIGINPROBLEM_H