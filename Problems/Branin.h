//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_BRANIN_H
#define SF2565_PROJECT_BRANIN_H

#include "../Problem.h"
#include <cmath>

struct BraninProblem final : public Problem {
    BraninProblem()
    : Problem(std::vector<double>{-5.0, 0.0},
              std::vector<double>{ 10.0, 15.0}) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();
        if (x.size() != 2) {
            throw std::invalid_argument("BraninProblem expects dimension = 2.");
        }

        const double x1 = x[0];
        const double x2 = x[1];

        constexpr double pi = std::numbers::pi_v<double>;

        const double term = x2
                          - (5.1 / (4.0 * pi * pi)) * x1 * x1
                          + (5.0 / pi) * x1
                          - 6.0;

        const double value = term * term
                           + 10.0 * (1.0 - 1.0 / (8.0 * pi)) * std::cos(x1)
                           + 10.0;

        return value;
    }
};




#endif //SF2565_PROJECT_BRANIN_H