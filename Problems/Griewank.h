//
// Created by quinten on 12/7/25.
//

#ifndef SF2565_PROJECT_GRIEWANK_H
#define SF2565_PROJECT_GRIEWANK_H


#pragma once
#include <vector>
#include <cmath>
#include "../Problem.h"

// Griewank function (multimodal, mild epistasis)
// f(x) = 1 + sum_{i=1}^d (x_i^2 / 4000) - prod_{i=1}^d cos(x_i / sqrt(i))
// Global minimum at x = 0 with f = 0
// Typical bounds: [-600, 600]^d
struct GriewankProblem final : public Problem {
    explicit GriewankProblem(const int dimension)
        : Problem(std::vector<double>(dimension, -600.0),
                  std::vector<double>(dimension,  600.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        const int d = static_cast<int>(x.size());
        if (d == 0) return 0.0;

        double sum = 0.0;
        double prod = 1.0;
        for (int i = 0; i < d; ++i) {
            const double xi = x[i];
            sum += (xi * xi) / 4000.0;
            const double angle = xi / std::sqrt(static_cast<double>(i + 1));
            prod *= std::cos(angle);
        }
        return 1.0 + sum - prod;  // min = 0 at x = 0
    }
};




#endif //SF2565_PROJECT_GRIEWANK_H