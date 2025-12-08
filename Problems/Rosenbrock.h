//
// Created by quinten on 12/7/25.
//

#ifndef SF2565_PROJECT_ROSENBROCK_H
#define SF2565_PROJECT_ROSENBROCK_H

#pragma once
#include <vector>
#include "../Problem.h"


// Rosenbrock function (banana)
// f(x) = sum_{i=1}^{d-1} [ 100*(x_{i+1} - x_i^2)^2 + (1 - x_i)^2 ]
// Global minimum: f(1,...,1) = 0
// Typical bounds: [-5, 10]^d
struct RosenbrockProblem final : public Problem {
    explicit RosenbrockProblem(const int dimension)
        : Problem(std::vector<double>(dimension, -5.0),
                  std::vector<double>(dimension, 10.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();

        const int d = static_cast<int>(x.size());
        double s = 0.0;
        // If d < 2, the sum is empty -> 0 (but use d >= 2 in tests)
        for (int i = 0; i + 1 < d; ++i) {
            const double xi   = x[i];
            const double xip1 = x[i + 1];
            const double t1 = (xip1 - xi*xi);
            const double t2 = (1.0 - xi);
            s += 100.0 * t1 * t1 + t2 * t2;
        }
        return s;
    }
};


#endif //SF2565_PROJECT_ROSENBROCK_H