//
// Created by quinten on 12/5/25.
//

#ifndef SF2565_PROJECT_SPHEREPROBLEM_H
#define SF2565_PROJECT_SPHEREPROBLEM_H

#include "../Problem.h"

struct SphereProblem final : public Problem {
    explicit SphereProblem(const int dimension) : Problem(
        std::vector<double>(dimension, -5.0),
        std::vector<double>(dimension, 5.0)
        ) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        double sum=0.0;
        for (const auto v : x) { sum += v*v; }
        return sum; // solution if 0 at x=0
    }
};


#endif //SF2565_PROJECT_SPHEREPROBLEM_H