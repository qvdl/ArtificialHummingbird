//
// Created by quinten on 12/7/25.
//

#ifndef SF2565_PROJECT_GRIEWANK_H
#define SF2565_PROJECT_GRIEWANK_H


#pragma once
#include <vector>
#include <cmath>
#include "../Problem.h"

// (weakly) MULTIMODAL
// solution f=0 at x=0

struct GriewankProblem final : public Problem {
    explicit GriewankProblem(const int dimension=30)
        : Problem(std::vector<double>(dimension, -600.0),
                  std::vector<double>(dimension,  600.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();

        const int d = static_cast<int>(x.size());
        if (d == 0) return 0.0;

        double sum = 0.0;
        double prod = 1.0;
        for (int i = 0; i < d; ++i) {
            const double xi = x[i];
            const double xi_shifted = xi-100.0;
            sum += (xi_shifted * xi_shifted) / 4000.0;
            const double angle = xi_shifted / std::sqrt(static_cast<double>(i + 1));
            prod *= std::cos(angle);
        }
        return 1.0 + sum - prod;
    }
};




#endif //SF2565_PROJECT_GRIEWANK_H