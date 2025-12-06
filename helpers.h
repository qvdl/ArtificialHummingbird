//
// Created by quinten on 12/6/25.
//

#ifndef SF2565_PROJECT_HELPERS_H
#define SF2565_PROJECT_HELPERS_H

#include <vector>
#include "Problem.h"

// --- STRUCTS ---
struct Result {
    double best_f;
    std::vector<double> best_x;
};


struct Expectation {
    std::string name;
    bool pass;
    double f;
    double x_err;
    int dim, pop, iters; unsigned seed;
};



// --- FUNCTIONS ---
double l2norm(const std::vector<double>& x);
double l2dist(const std::vector<double>& a, const std::vector<double>& b);
Result run_once(const Problem& prob, int pop, int iters, unsigned seed);




#endif //SF2565_PROJECT_HELPERS_H