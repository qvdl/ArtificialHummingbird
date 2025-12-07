//
// Created by quinten on 12/7/25.
//

#ifndef SF2565_PROJECT_SCHWEFEL226_H
#define SF2565_PROJECT_SCHWEFEL226_H


#pragma once
#include <vector>
#include <cmath>
#include "../Problem.h"

// Schwefel 2.26 (MN) — multimodal, nonseparable
// f(x) = A * d - sum_{i=1..d} x_i * sin(sqrt(|x_i|))
// Global minimum at x_i ≈ 420.968746..., with f = 0
// Typical bounds: [-500, 500]^d
struct Schwefel226MNProblem final : public Problem {
    explicit Schwefel226MNProblem(const int dimension)
        : Problem(std::vector<double>(dimension, -500.0),
                  std::vector<double>(dimension,  500.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        const int d = static_cast<int>(x.size());
        if (d == 0) return 0.0;

        // Standard constant used in literature (double precision)
        constexpr double A = 418.9828872724338;

        double sum = 0.0;
        for (int i = 0; i < d; ++i) {
            const double xi = x[i];
            sum += xi * std::sin(std::sqrt(std::fabs(xi)));
        }
        return A * d - sum; // minimum ~0 at x_i ≈ 420.968746...
    }
};



#endif //SF2565_PROJECT_SCHWEFEL226_H