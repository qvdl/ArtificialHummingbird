#pragma once
#include <vector>
#include <string>
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

// one-call sanity tests for Sphere / Abs / ShiftedQuadratic / Rastrigin
void sanity_tests(int pop, int iters, int dimension, unsigned seed);