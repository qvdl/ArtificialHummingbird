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


struct Schwefel226Problem final : public Problem {
    explicit Schwefel226Problem(const int dimension=30)
        : Problem(std::vector<double>(dimension, -500.0),
                  std::vector<double>(dimension,  500.0)) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();

        const int d = static_cast<int>(x.size());

        double sum = 0.0;
        for (int i = 0; i < d; ++i) {
            const double xi = x[i];
            sum += xi * std::sin(std::sqrt(std::fabs(xi)));
        }
        return -sum;
    }
};



#endif //SF2565_PROJECT_SCHWEFEL226_H