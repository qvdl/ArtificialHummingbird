//
// helpers.cpp — utility routines for running and validating AHA optimization
//

#include "helpers.h"
#include <cmath>
#include "RNG_Utils.h"
#include "AHA_Optimizer.h"
#include <iostream>

// Compute Euclidean norm ‖x‖₂.
double l2norm(const std::vector<double>& x) {
    double s = 0;
    for (const double v : x) s += v * v;
    return std::sqrt(s);
}

// Euclidean distance between two points.
double l2dist(const std::vector<double>& a, const std::vector<double>& b) {
    double s = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        const double d = a[i] - b[i];
        s += d * d;
    }
    return std::sqrt(s);
}

// Run a single AHA optimization with a fixed seed.
// Returns both final fitness and solution vector.
Result run_once(const Problem& prob, const int pop, const int iters, const unsigned seed)
{
    RNG_Utils::initialize(seed);   // ensure deterministic behavior per test
    AHA_Optimizer opt(prob, pop, iters);
    opt.solve();
    return { opt.getGlobalBestFitness(), opt.getGlobalBestPosition() };
}

// Run an optimization and check whether the solution converges to zero.
void run_and_check_to_zero(Problem& prob, int pop, int iters,
                           double tol_f, double tol_x, const char* label)
{
    AHA_Optimizer opt(prob, pop, iters);
    opt.solve();

    double f    = opt.getGlobalBestFitness();
    double xerr = l2norm(opt.getGlobalBestPosition());

    // PASS if fitness and distance from zero are within tolerances.
    std::cout << ((f <= tol_f && xerr <= tol_x) ? "[PASS] " : "[FAIL] ")
              << label << " f=" << f << " x_err=" << xerr << "\n";
}

// Run an optimization and check whether it converges to a point.
void run_and_check_to_point(Problem& prob, int pop, int iters,
                            const std::vector<double>& target,
                            double tol_f, double tol_x, const char* label)
{
    AHA_Optimizer opt(prob, pop, iters);
    opt.solve();

    double f    = opt.getGlobalBestFitness();
    double xerr = l2dist(opt.getGlobalBestPosition(), target);

    // PASS if both the final fitness and position error match expectations.
    std::cout << ((f <= tol_f && xerr <= tol_x) ? "[PASS] " : "[FAIL] ")
              << label << " f=" << f << " x_err=" << xerr << "\n";
}