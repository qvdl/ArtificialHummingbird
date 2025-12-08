#include <memory>
#include <vector>
#include <string>
#include "Problems/Rosenbrock.h"
#include "Problems/Sphere.h"
#include "Problems/AbsoluteValue.h"
#include "Problems/Griewank.h"
#include "Problems/Rastrigin.h"
#include "Problems/Schwefel12.h"
#include "Problems/Schwefel226.h"
#include "Problems/ShiftedQuadratic.h"
#include <functional>
#include "helpers.h"

int main() {
    // Experiment knobs (keep simple & consistent)
    constexpr int population_size = 50;
    constexpr int nr_iterations   = 10'000;
    constexpr int dim             = 10;
    constexpr int budget_mult     = 20'000;
    const int budget              = budget_mult * dim;
    const char* out_csv           = "../results_aha2.csv";

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
                out_csv,          // appends one CSV row
                name,
                dim,
                seed,
                budget,
                *prob,
                population_size,
                nr_iterations,
                xstar(dim)      // used for x_err in your logger
            );
        }
    }
    return 0;
}
