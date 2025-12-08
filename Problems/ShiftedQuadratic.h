//
// Created by quinten on 12/5/25.
//

#ifndef SF2565_PROJECT_SHIFTEDQUADRATIC_H
#define SF2565_PROJECT_SHIFTEDQUADRATIC_H

#include "../Problem.h"

struct ShiftedQuadraticProblem final : public Problem {
    explicit ShiftedQuadraticProblem() : Problem( {-10.0, -10.0}, {10.0, 10.0} ) {}

    [[nodiscard]] double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        return (x[0]-3.0)*(x[0]-3.0) + (x[1]+2.0)*(x[1]+2.0); // minimum is 0 at x=(3,-2)
    }
};

#endif //SF2565_PROJECT_SHIFTEDQUADRATIC_H