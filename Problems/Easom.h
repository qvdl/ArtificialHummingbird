//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_EASOM_H
#define SF2565_PROJECT_EASOM_H



#pragma once
#include <vector>
#include <cmath>
#include <stdexcept>
#include <numbers>
#include "../Problem.h"

struct EasomProblem final : public Problem {
    EasomProblem()
    : Problem(std::vector<double>{-100.0, -100.0},
              std::vector<double>{ 100.0,  100.0}) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();
        if (x.size() != 2) {
            throw std::invalid_argument("EasomProblem expects dimension = 2.");
        }

        const double x1 = x[0];
        const double x2 = x[1];
        constexpr double pi = std::numbers::pi_v<double>;

        const double dx1 = x1 - pi;
        const double dx2 = x2 - pi;

        return -std::cos(x1) * std::cos(x2) * std::exp(-(dx1 * dx1 + dx2 * dx2));
    }
};



#endif //SF2565_PROJECT_EASOM_H