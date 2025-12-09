//
// Created by quinten on 12/5/25.
//

#ifndef SF2565_PROJECT_SPHEREPROBLEM_H
#define SF2565_PROJECT_SPHEREPROBLEM_H

#include "../Problem.h"

// UNIMODAL -- SEPERABLE
// default dimentions: 30
// solution f=0 at x=0

struct SphereProblem final : public Problem {
    explicit SphereProblem(const int dimension=30) : Problem(
        std::vector<double>(dimension, -100.0),
        std::vector<double>(dimension, 100.0)
        ) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        double sum=0.0;
        for (const auto v : x) { sum += v*v; }
        return sum;
    }
};


#endif //SF2565_PROJECT_SPHEREPROBLEM_H