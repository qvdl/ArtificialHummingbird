#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "Problem.h"
#include "AHA_Optimizer.h"
#include "RNG_Utils.h"

#include "Problems/AbsoluteValue.h"
#include "Problems/Ackley.h"
#include "Problems/Beale.h"
#include "Problems/Branin.h"
#include "Problems/Easom.h"
#include "Problems/Griewank.h"
#include "Problems/Michalewicz10.h"
#include "Problems/Powell.h"
#include "Problems/Rastrigin.h"
#include "Problems/Rosenbrock.h"
#include "Problems/Schwefel12.h"
#include "Problems/Schwefel226.h"
#include "Problems/Shekel10.h"
#include "Problems/ShiftedQuadratic.h"
#include "Problems/SixHumpCamelBack.h"
#include "Problems/Sphere.h"
#include "Problems/Zakharov.h"


// ========= small helpers =========
struct Stats {
  double mean   = 0.0;
  double stdev  = 0.0;   // population std (matches paper Eq. 15)
  double best   = 0.0;
  double median = 0.0;
  double worst  = 0.0;
};

static Stats summarize(std::vector<double> v) {
  const int n = static_cast<int>(v.size());
  if (n == 0) return {};
  std::ranges::sort(v);
  const double sum = std::accumulate(v.begin(), v.end(), 0.0);
  const double mean = sum / n;
  double var = 0.0;                 // population variance
  for (const double x : v) var += (x - mean) * (x - mean);
  var /= n;
  Stats s;
  s.mean = mean;
  s.stdev = std::sqrt(var);
  s.best = v.front();
  s.worst = v.back();
  s.median = (n % 2) ? v[n/2] : 0.5 * (v[n/2 - 1] + v[n/2]);
  return s;
}

// ========= hook to your optimizer =========
// Approximates FE budget: each iteration ≈ population evaluations,
// so choose maxIterations ≈ FE / population.
static double run_aha_once(Problem& prob,
                           int population,
                           int fe_budget,
                           unsigned seed) {
  RNG_Utils::initialize(seed);

  const int maxIterations = std::max(1, fe_budget / population);

  AHA_Optimizer aha(prob, population, maxIterations);
  aha.solve();  // your solve() updates global best internally

  return aha.getGlobalBestFitness();
}

// ========= optional: put paper numbers here for Δ-printing =========
struct PaperRow { double mean; double stdev; };
using PaperTable = std::map<std::string, PaperRow>;

static PaperTable paper_stats = {
  {"Ackley(d=30)",        { 8.88e-16, 0.0 }},
  {"Beale(d=2)", { 0.0, 0.0}},
  {"Branin(d=2)",         {0.39788735772973816, 0.0}},
  {"Easom(d=2)",          {-1.0, 0.0}},
  {"Griewank(d=30)",      { 0.0, 0.0 }},
  {"Michalewicz10(d=10)", { -9.6591914, 1.88e-2 }},
  {"Powell(d=24)",        { 4.25e-294, 0.0 }},
  {"Rastrigin(d=30)",     { 0.0, 0.0 }},
  {"Rosenbrock(d=30)",    {25.065057, 0.278139}},
  {"Schwefel12(d=30)", { 2.96e-278, 0.0 }},
  {"Schwefel226(d=30)",   { -12409.83, 225.97788 }},
  {"Shekel10(d=4)",       {-10.53641, 0.0}},
  {"SixHumpCamelBack(d=2)", { -1.031628, 6.65e-16 }},
  {"Sphere(d=30)",        {7.32e-292,        0.0}},
  {"Zakharov(d=10)", { 1.78e-262, 0.0 }},
};

// ========= main driver =========
int main(int argc, char** argv) {
  // --- global config (adjust if you want) ---
  const int   R           = 30;          // independent runs (paper uses 30)
  const int   population  = 50;          // paper default
  const int   FE_budget   = 50000;       // ~ paper budget per function
  const auto  base_seed   = 20251209u;   // reproducible base seed
  const char* csv_path    = "../aha_bench_results.csv";

  struct Item { std::string name; std::unique_ptr<Problem> ptr; };
  std::vector<Item> problems;
  problems.reserve(32);

  // ===== REGISTER PROBLEMS HERE (add/remove freely) =====
  problems.push_back({"Ackley(d=30)",        std::make_unique<AckleyProblem>(30)});
  problems.push_back({"Beale(d=2)",            std::make_unique<BealeProblem>()});
  problems.push_back({"Branin(d=2)",           std::make_unique<BraninProblem>()});
  problems.push_back({"Easom(d=2)",            std::make_unique<EasomProblem>()});
  problems.push_back({"Griewank(d=30)",      std::make_unique<GriewankProblem>(30)});
  problems.push_back({"Michalewicz10(d=10)",   std::make_unique<Michalewicz10Problem>()});
  problems.push_back({"Powell(d=24)",          std::make_unique<PowellProblem>(24)});
  problems.push_back({"Rastrigin(d=30)",     std::make_unique<RastriginProblem>(30)});
  problems.push_back({"Rosenbrock(d=30)",    std::make_unique<RosenbrockProblem>(30)});
  problems.push_back({"Schwefel12(d=30)",   std::make_unique<Schwefel12Problem>(30)});
  problems.push_back({"Schwefel226(d=30)",   std::make_unique<Schwefel226Problem>(30)});
  problems.push_back({"Shekel10(d=4)",         std::make_unique<Shekel10Problem>()});
  problems.push_back({"SixHumpCamelBack(d=2)", std::make_unique<SixHumpCamelBackProblem>()});
  problems.push_back({"Sphere(d=30)",        std::make_unique<SphereProblem>(30)});
  problems.push_back({"Zakharov(d=10)",      std::make_unique<ZakharovProblem>(30)});


  // --- CSV header ---
  std::ofstream csv(csv_path);
  if (!csv) {
    std::cerr << "ERROR: cannot open CSV for writing: " << csv_path << "\n";
    return 1;
  }
  csv << "problem,D,R,FE,pop,mean,std,best,median,worst\n";
  csv << std::setprecision(16);

  // --- run all problems ---
  for (auto& item : problems) {
    Problem& prob = *item.ptr;
    const int D = static_cast<int>(prob.dimension());

    std::vector<double> results;
    results.reserve(R);

    for (int r = 0; r < R; ++r) {
      prob.resetEvalCount();                  // clean eval counter (if you track it)
      const unsigned seed = base_seed + static_cast<unsigned>(r);
      const double gbest = run_aha_once(prob, population, FE_budget, seed);
      results.push_back(gbest);
    }

    const Stats s = summarize(results);

    // CSV row
    csv << item.name << ","
        << D << ","
        << R << ","
        << FE_budget << ","
        << population << ","
        << s.mean << ","
        << s.stdev << ","
        << s.best << ","
        << s.median << ","
        << s.worst << "\n";

    // Console summary (+ optional Δ vs paper)
    auto it = paper_stats.find(item.name);
    std::cout << std::fixed << std::setprecision(6);
    if (it != paper_stats.end()) {
      const auto [pmean, pstd] = it->second;
      const double d_mean = s.mean - pmean;
      const double d_std  = s.stdev - pstd;
      std::cout << "[COMPARE] " << item.name
                << "  Mean=" << s.mean << " (Δ " << d_mean << ")"
                << "  Std="  << s.stdev << " (Δ " << d_std  << ")"
                << "  Best=" << s.best
                << "  Median=" << s.median
                << "  Worst=" << s.worst
                << "\n";
    } else {
      std::cout << "[RESULT]  " << item.name
                << "  Mean=" << s.mean
                << "  Std="  << s.stdev
                << "  Best=" << s.best
                << "  Median=" << s.median
                << "  Worst=" << s.worst
                << "\n";
    }
  }

  std::cout << "\nWrote CSV: " << csv_path << "\n";
  return 0;
}
