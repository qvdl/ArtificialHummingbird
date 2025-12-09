//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_SIXHUMPCAMELBACK_H
#define SF2565_PROJECT_SIXHUMPCAMELBACK_H

#include "../Problem.h"


struct SixHumpCamelBackProblem final : public Problem {
    explicit SixHumpCamelBackProblem()
        : Problem(std::vector<double>{-5.0, -5.0}, std::vector<double>{5.0,5.0}) {}


    double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        if (x.size() != 2) { throw std::invalid_argument("Six hump camel back problem expects dimension=2!"); }
        double const x1 = x[0];
        double const x2 = x[1];

        return 4.0*x1*x1 - 2.1*x1*x1*x1*x1 + x1*x1*x1*x1*x1*x1/3.0 + x1*x2 - 4.0*x2*x2 + 4.0*x2*x2*x2*x2;
    }
};


#endif //SF2565_PROJECT_SIXHUMPCAMELBACK_H