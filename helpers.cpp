//
// Created by quinten on 12/6/25.
//

#include "helpers.h"
#include <cmath>
#include "RNG_Utils.h"
#include "AHA_Optimizer.h"

double l2norm(const std::vector<double>& x) {
    double s=0; for(const double v : x) s += v*v; return std::sqrt(s);
}

double l2dist(const std::vector<double>& a, const std::vector<double>& b) {
    double s=0; for(size_t i=0;i<a.size();++i){ const double d=a[i]-b[i]; s+=d*d; }
    return std::sqrt(s);
}

Result run_once(const Problem& prob, const int pop, const int iters, const unsigned seed)
{
    RNG_Utils::initialize(seed);
    AHA_Optimizer opt(prob, pop, iters);
    opt.solve();
    return { opt.getGlobalBestFitness(), opt.getGlobalBestPosition() };
}

