//
// Created by quinten on 12/7/25.
//

#ifndef SF2565_PROJECT_SCHWEFEL_H
#define SF2565_PROJECT_SCHWEFEL_H


#pragma once
#include <vector>
#include "../Problem.h"

// Schwefel 1.2 (Cumulative Sum of Squares), unimodal, non-separable.
// Global minimum at x = 0 with f = 0.

struct Schwefel12Problem final : public Problem {
    explicit Schwefel12Problem(const int dimension=30)
        : Problem(std::vector<double>(dimension, -100.0),
                  std::vector<double>(dimension,  100.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();

        const int d = static_cast<int>(x.size());
        double s = 0.0;

        for (int i=0; i<d; ++i) {
            double s_inside = 0.0;
            for (int j=0; j<i; ++j) {
                const double xj = x[j];
                s_inside += xj;
            }
            s += s_inside * s_inside;
        }

        return s;
    }
};


#endif //SF2565_PROJECT_SCHWEFEL_H