//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_ACKLEY_H
#define SF2565_PROJECT_ACKLEY_H

#include "../Problem.h"
#include <cmath>

struct AckleyProblem final : public Problem {
    explicit AckleyProblem(const int dimension=30)
    : Problem(std::vector<double>(dimension, -32.0),
          std::vector<double>(dimension,  32.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        double s1 = 0.0;
        double s2 = 0.0;
        const int d = static_cast<int>(x.size());
        constexpr double two_pi = 2.0 * M_PI;

        for (int i=0; i<d; ++i) {
            const double xi = x[i];
            s1 += xi*xi / d;
            s2 += std::cos(two_pi * xi) / d;
        }

        return -20.0*std::exp(-0.2 * std::sqrt(s1)) - std::exp(s2) + 20.0 + std::numbers::e;
    }
};

#endif //SF2565_PROJECT_ACKLEY_H