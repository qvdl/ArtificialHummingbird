//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_MICHALEWICZ10_H
#define SF2565_PROJECT_MICHALEWICZ10_H

#include "../Problem.h"
#include <cmath>

struct Michalewicz10Problem final : Problem {
    explicit Michalewicz10Problem(const int dimension=10)
    : Problem(std::vector<double>(dimension, 0.0),
          std::vector<double>(dimension,  M_PI)) {}


    double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        double s=0.0;
        const int d = static_cast<int>(x.size());

        for (int i=0; i<d; ++i) {
            const double xi=x[i];
            const double term1 = std::sin(xi);
            const double term2 = std::sin((i+1) * xi * xi / M_PI);
            s += term1 * std::pow(term2, 20);
        }

        return -s;
    }
};

#endif //SF2565_PROJECT_MICHALEWICZ10_H