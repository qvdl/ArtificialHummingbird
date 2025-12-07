#include "helpers.h"
#include <cmath>
#include "RNG_Utils.h"
#include "AHA_Optimizer.h"
#include <iostream>
#include <chrono>

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
void run_and_check_to_zero(const Problem& prob, const int pop, const int iters,
                           const double tol_f, const double tol_x, const char* label)
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
void run_and_check_to_point(const Problem& prob, const int pop, const int iters, const std::vector<double>& target,
                            const double tol_f, const double tol_x, const char* label)
{
    AHA_Optimizer opt(prob, pop, iters);
    opt.solve();

    double f    = opt.getGlobalBestFitness();
    double xerr = l2dist(opt.getGlobalBestPosition(), target);

    // PASS if both the final fitness and position error match expectations.
    std::cout << ((f <= tol_f && xerr <= tol_x) ? "[PASS] " : "[FAIL] ")
              << label << " f=" << f << " x_err=" << xerr << "\n";
}


// Usage demo: run AHA on any Problem and print a short summary.
void demo_usage(const Problem& prob, int populationSize, int iterations, unsigned seed)
{
    RNG_Utils::initialize(seed);
    AHA_Optimizer opt(prob, populationSize, iterations);
    opt.solve();

    std::cout << "\n[DEMO] f*=" << opt.getGlobalBestFitness() << "  x*=[";
    const auto& x = opt.getGlobalBestPosition();
    for (size_t i = 0; i < x.size(); ++i) {
        std::cout << x[i] << (i + 1 < x.size() ? ", " : "");
    }
    std::cout << "]\n";
}




#include "Problems/Sphere.h"
#include "Problems/AbsoluteValue.h"
#include "Problems/ShiftedQuadratic.h"
#include "Problems/Rastrigin.h"

void sanity_tests(const int pop, const int iters, const int dimension, const unsigned seed)
{
    std::cout << "-- starting sanity tests --\nPopulation size: " << pop << "\nNr. of iterations: " << iters << "\nDimension: " << dimension << "\n[seed=" << seed << "]\n\n";
    constexpr double tol_f_zero = 1e-12, tol_x_zero = 1e-12;

    // Sphere (d=10): optimum at 0
    {
        const SphereProblem prob(dimension);
        RNG_Utils::initialize(seed);
        run_and_check_to_zero(prob, pop, iters, tol_f_zero, tol_x_zero, "Sphere(d=10)");
    }

    // Absolute value (d=10): optimum at 0
    {
        const AbsoluteValueProblem prob(dimension);
        RNG_Utils::initialize(seed);
        run_and_check_to_zero(prob, pop, iters, tol_f_zero, tol_x_zero, "Abs(d=10)");
    }

    // ShiftedQuadratic (2D): optimum at (3,-2)
    {
        constexpr double tol_x_pt   = 1e-12;
        constexpr double tol_f_pt   = 1e-12;
        const ShiftedQuadraticProblem prob;
        RNG_Utils::initialize(seed);
        run_and_check_to_point(prob, pop, iters, {3.0, -2.0}, tol_f_pt, tol_x_pt, "ShiftedQuadratic");
    }

    // Rastrigin (d=10)
    {
        const RastriginProblem prob(dimension);
        RNG_Utils::initialize(seed);

        auto [best_f, best_x] = run_once(prob, pop, iters, seed);

        const double f    = best_f;
        const double xerr = l2norm(best_x);   // distance to optimum at zero

        constexpr double tol_f_rast = 1e-4;
        constexpr double tol_x_rast = 1e-4;

        std::cout << ((f <= tol_f_rast && xerr <= tol_x_rast) ? "[PASS] " : "[FAIL] ")
                  << "Rastrigin(d=10) f=" << f << " x_err=" << xerr << "\n";
    }

    std::cout << "-- sanity tests finished --\n\n";
}
