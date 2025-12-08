#pragma once
#include <vector>
#include "Problem.h"


struct Result { double best_f; std::vector<double> best_x; };

double l2norm(const std::vector<double>& x);
double l2dist(const std::vector<double>& a, const std::vector<double>& b);

Result run_once(const Problem& prob, int pop, int iters, unsigned seed);

// Zero/point checks (keep for quick assertions)
void run_and_check_to_zero(const Problem& prob, int pop, int iters, double tol_f, double tol_x, const char* label);
void run_and_check_to_point(const Problem& prob, int pop, int iters, const std::vector<double>& target,
                            double tol_f, double tol_x, const char* label);

// ultra-compact usage demo
void demo_usage(const Problem& prob, int populationSize, int iterations, unsigned seed);

// one-call sanity tests
void sanity_tests(int pop, int iters, int dimension, unsigned seed);


namespace aha_log {
    void write_csv_header(const std::string& path);

    void run_aha_and_log_csv(
        const std::string& csv_path,
        const std::string& problem_name,
        int dim,
        int seed,
        long long budget,
        Problem& problem,
        int populationSize,
        int maxIter,
        const std::vector<double>& x_star,
        bool use_taskflow = false,
        long long fevals_override = -1,
        const char* algo_label = "AHA"
    );
}