//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_POWELL_H
#define SF2565_PROJECT_POWELL_H

#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include "../Problem.h"

struct PowellProblem final : public Problem {
    explicit PowellProblem(int const dimension = 24)
    : Problem(std::vector<double>(dimension, -4.0),
              std::vector<double>(dimension,  5.0)) {
        if (dimension % 4 != 0) {
            throw std::invalid_argument("PowellProblem dimension must be a multiple of 4.");
        }
    }

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();
        const int d = static_cast<int>(x.size());
        if (d % 4 != 0) {
            throw std::invalid_argument("PowellProblem: x.size() must be a multiple of 4.");
        }

        double f = 0.0;
        for (int i = 0; i < d; i += 4) {
            const double x1 = x[i + 0];
            const double x2 = x[i + 1];
            const double x3 = x[i + 2];
            const double x4 = x[i + 3];

            const double t1 = (x1 + 10.0 * x2);
            const double t2 = (x3 - x4);
            const double t3 = (x2 - 2.0 * x3);
            const double t4 = (x1 - x4);

            f += t1 * t1
               + 5.0 * (t2 * t2)
               + std::pow(t3, 4)
               + 10.0 * std::pow(t4, 4);
        }
        return f;
    }
};


#endif //SF2565_PROJECT_POWELL_H