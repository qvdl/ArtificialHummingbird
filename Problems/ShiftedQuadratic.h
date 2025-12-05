//
// Created by quinten on 12/5/25.
//

#ifndef SF2565_PROJECT_SHIFTEDQUADRATIC_H
#define SF2565_PROJECT_SHIFTEDQUADRATIC_H

#include "../Problem.h"

struct ShiftedQuadratic final : public Problem {
    explicit ShiftedQuadratic() : Problem( {-10.0, -10.0}, {10.0, 10.0} ) {}

    double evaluate(std::vector<double> &x) const override {
        return (x[0]-3.0)*(x[0]-3.0) + (x[1]+2.0)*(x[1]+2.0); // minimum is 0 at x=(3,-2)
    }
};

#endif //SF2565_PROJECT_SHIFTEDQUADRATIC_H