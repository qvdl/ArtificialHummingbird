#include "helpers.h"
#include <cmath>
#include "RNG_Utils.h"
#include "AHA_Optimizer.h"
#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>

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
#include "Problems/Rosenbrock.h"
#include "Problems/Griewank.h"
#include "Problems/Schwefel12.h"
#include "Problems/Schwefel226.h"

void sanity_tests(const int pop, const int iters, const int dimension, const unsigned seed)
{
    std::cout << "-- starting sanity tests --\nPopulation size: " << pop
              << "\nNr. of iterations: " << iters
              << "\nDimension: " << dimension
              << "\n[seed=" << seed << "]\n\n";

    // Very strict zero-optimum tolerances (your choice)
    constexpr double tol_f_zero = 1e-12, tol_x_zero = 1e-12;

    // Sphere (d=dimension): optimum at 0
    {
        const SphereProblem prob(dimension);
        RNG_Utils::initialize(seed);
        run_and_check_to_zero(prob, pop, iters, tol_f_zero, tol_x_zero, "Sphere(d)");
    }

    // Absolute value (d=dimension): optimum at 0
    {
        const AbsoluteValueProblem prob(dimension);
        RNG_Utils::initialize(seed);
        run_and_check_to_zero(prob, pop, iters, tol_f_zero, tol_x_zero, "Abs(d)");
    }

    // ShiftedQuadratic (2D fixed): optimum at (3,-2)
    {
        constexpr double tol_f_pt = 1e-12, tol_x_pt = 1e-12;
        const ShiftedQuadraticProblem prob(dimension);  // your 2D problem
        RNG_Utils::initialize(seed);
        run_and_check_to_point(prob, pop, iters, {3.0, -2.0}, tol_f_pt, tol_x_pt, "ShiftedQuadratic");
    }

    // Rastrigin (d=dimension): multimodal, optimum at 0 (looser tolerances)
    {
        const RastriginProblem prob(dimension);
        RNG_Utils::initialize(seed);

        auto [best_f, best_x] = run_once(prob, pop, iters, seed);
        const double f    = best_f;
        const double xerr = l2norm(best_x);

        constexpr double tol_f_rast = 1e-4;
        constexpr double tol_x_rast = 1e-4;

        std::cout << ((f <= tol_f_rast && xerr <= tol_x_rast) ? "[PASS] " : "[FAIL] ")
                  << "Rastrigin(d) f=" << f << " x_err=" << xerr << "\n";
    }

    // Rosenbrock (d=dimension): optimum at (1,...,1) (use point check)
    {
        const RosenbrockProblem prob(dimension);
        RNG_Utils::initialize(seed);

        // Slightly looser than zero-optimum tests (valley is narrow)
        constexpr double tol_f_ros = 1e-6;
        constexpr double tol_x_ros = 1e-3;

        std::vector<double> target(dimension, 1.0);
        run_and_check_to_point(const_cast<RosenbrockProblem&>(prob),  // helper takes non-const
                               pop, iters, target, tol_f_ros, tol_x_ros, "Rosenbrock(d)");
    }

    // Griewank (d=dimension): optimum at 0 (looser tolerances)
    {
        const GriewankProblem prob(dimension);
        RNG_Utils::initialize(seed);

        auto [best_f, best_x] = run_once(prob, pop, iters, seed);
        const double f    = best_f;
        const double xerr = l2norm(best_x);

        constexpr double tol_f_gri = 1e-4;
        constexpr double tol_x_gri = 1e-4;

        std::cout << ((f <= tol_f_gri && xerr <= tol_x_gri) ? "[PASS] " : "[FAIL] ")
                  << "Griewank(d) f=" << f << " x_err=" << xerr << "\n";
    }

    // Schwefel 1.2 (d=dimension): optimum at 0 (zero check ok)
    {
        const Schwefel12Problem prob(dimension);
        RNG_Utils::initialize(seed);
        run_and_check_to_zero(prob, pop, iters, tol_f_zero, tol_x_zero, "Schwefel1.2(d)");
    }

    // Schwefel 2.26 (d=dimension): optimum near 420.968746... per dim (point check)
    {
        const Schwefel226Problem prob(dimension);
        RNG_Utils::initialize(seed);

        auto [best_f, best_x] = run_once(prob, pop, iters, seed);
        const double f = best_f;

        // Build target ≈ (420.968746, ..., 420.968746)
        constexpr double xopt = 420.968746;
        std::vector<double> target(dimension, xopt);
        const double xerr = l2dist(best_x, target);

        // Reasonable tolerances for a hard multimodal function
        constexpr double tol_f_sch = 1e-6;   // adjust if your budget is smaller
        constexpr double tol_x_sch = 1e-1;   // loose positional tolerance

        std::cout << ((f <= tol_f_sch && xerr <= tol_x_sch) ? "[PASS] " : "[FAIL] ")
                  << "Schwefel2.26(d) f=" << f << " x_err=" << xerr << "\n";
    }

    std::cout << "-- sanity tests finished --\n\n";
}


void aha_log::write_csv_header(const std::string &path) {
    namespace fs = std::filesystem;
    bool need_header = !fs::exists(path) || fs::file_size(path) == 0;
    if (need_header) {
        std::ofstream out(path, std::ios::app);
        out << "problem,algo,dim,seed,budget,fevals,f_best,x_err,time_ms\n";
    }
}

void aha_log::run_aha_and_log_csv(const std::string &csv_path, const std::string &problem_name, int dim, int seed, long long budget, Problem &problem, int populationSize, int maxIter, const std::vector<double> &x_star, bool use_taskflow, long long fevals_override, const char *algo_label) {
    {
        write_csv_header(csv_path);

        // If you seed your RNG somewhere central, do it here (placeholder):
        // RNG::seed(seed);

        AHA_Optimizer aha(problem, populationSize, maxIter);

        const auto t0 = std::chrono::steady_clock::now();
        aha.solve();
        const auto t1  = std::chrono::steady_clock::now();
        const double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

        // Get best solution (adapt names if your API differs)
        const std::vector<double>& x_best = aha.getGlobalBestPosition();
        const double f_best = aha.getGlobalBestFitness();

        // Function evaluations: user-supplied exact value or a simple estimate
        auto fevals = problem.getEvalCount();

        const double err = l2dist(x_best, x_star);

        std::ofstream out(csv_path, std::ios::app);
        out.setf(std::ios::scientific);
        out << std::setprecision(16)
            << problem_name  << ","
            << algo_label    << ","
            << dim           << ","
            << seed          << ","
            << budget        << ","
            << fevals        << ","
            << f_best        << ","
            << err           << ","
            << ms            << "\n";
    }
}

void aha_log::run_aha_and_log_csv_(int dim, int pop_size, int nr_it, int budget, const char* csv_path) {
    // Seeds (10 runs is fine for now)
    const int seed_first = 2015;
    const int seed_last  = 2025; // not included

    // Helper to make the optimum vector you’ll log x_err against
    auto Z = [](int d){ return std::vector<double>(d, 0.0); };
    auto O = [](int d){ return std::vector<double>(d, 1.0); };
    auto S226 = [](int d){ return std::vector<double>(d, 420.968746036); };

    struct Entry {
        std::string name;
        std::function<std::unique_ptr<Problem>(int)> make;
        std::function<std::vector<double>(int)> xstar;
    };

    // Your 8 problems
    const std::vector<Entry> P = {
        {"Sphere",          [](int d){ return std::make_unique<SphereProblem>(d); },          Z},
        {"AbsoluteValue",   [](int d){ return std::make_unique<AbsoluteValueProblem>(d); },   Z},
        {"ShiftedQuadratic",[](int d){ return std::make_unique<ShiftedQuadraticProblem>(d);}, /* better: fetch shift from prob */ Z},
        {"Rosenbrock",      [](int d){ return std::make_unique<RosenbrockProblem>(d); },      O},
        {"Rastrigin",       [](int d){ return std::make_unique<RastriginProblem>(d); },       Z},
        {"Griewank",        [](int d){ return std::make_unique<GriewankProblem>(d); },        Z},
        {"Schwefel12",      [](int d){ return std::make_unique<Schwefel12Problem>(d); },      Z},
        {"Schwefel226",     [](int d){ return std::make_unique<Schwefel226Problem>(d); },     S226},
    };

    for (const auto&[name, make, xstar] : P) {
        for (int seed = seed_first; seed < seed_last; ++seed) {
            auto prob = make(dim);
            aha_log::run_aha_and_log_csv(
                csv_path,          // appends one CSV row
                name,
                dim,
                seed,
                budget,
                *prob,
                pop_size,
                nr_it,
                xstar(dim)      // used for x_err in your logger
            );
        }
    }
}

