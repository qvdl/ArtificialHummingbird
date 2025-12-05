//
// Created by quinten on 12/5/25.
//

#ifndef SF2565_PROJECT_ABSOLUTEVALUEPROBLEM_H
#define SF2565_PROJECT_ABSOLUTEVALUEPROBLEM_H

#include "../Problem.h"

struct AbsoluteValueProblem final : public Problem {
    explicit AbsoluteValueProblem(const int dimension) :
    Problem(
        std::vector<double>(dimension, -10.0),
        std::vector<double>(dimension, 10.0)
    ) {}

    double evaluate(std::vector<double> &x) const override {
        double sum=0.0;
        for (const auto v : x) { sum += std::abs(v); }
        return sum; // minimum is 0 at x=0
    }
};

#endif //SF2565_PROJECT_ABSOLUTEVALUEPROBLEM_H