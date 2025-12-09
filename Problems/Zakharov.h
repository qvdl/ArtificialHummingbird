//
// Created by quinten on 12/9/25.
//

#ifndef SF2565_PROJECT_ZAKHAROV_H
#define SF2565_PROJECT_ZAKHAROV_H

#include "../Problem.h"


struct ZakharovProblem final : public Problem {
    explicit ZakharovProblem(const int dimension=10)
    : Problem(std::vector<double>(dimension, -5.0),
              std::vector<double>(dimension,  10.0)) {}


    double evaluate(const std::vector<double> &x) const override {
        incrementEvalCount();

        double s1=0.0;
        double s2=0.0;
        const int d = static_cast<int>(x.size());

        for (int i=0; i<d; ++i) {
            const double xi = x[i];
            s1 += xi*xi;
            s2 += 0.5 * (i+1) * xi;
        }

        return s1 + s2*s2 + s2*s2*s2*s2;
    }
};

#endif //SF2565_PROJECT_ZAKHAROV_H