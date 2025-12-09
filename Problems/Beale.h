//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_BEALE_H
#define SF2565_PROJECT_BEALE_H


#include "../Problem.h"


struct BealeProblem final : public Problem {
    BealeProblem()
    : Problem({-4.5, -4.5}, {4.5, 4.5}) {}

    [[nodiscard]] double evaluate(const std::vector<double>& x) const override {
        incrementEvalCount();

        if (x.size() != 2)
            throw std::invalid_argument("Beale is 2-dimensional.");

        const double x1 = x[0];
        const double x2 = x[1];

        const double t1 = 1.5   - x1 + x1 * x2;
        const double t2 = 2.25  - x1 + x1 * x2 * x2;
        const double t3 = 2.625 - x1 + x1 * x2 * x2 * x2;

        return t1*t1 + t2*t2 + t3*t3;
    }
};

#endif //SF2565_PROJECT_BEALE_H