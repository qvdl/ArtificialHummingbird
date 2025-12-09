#include "helpers.h"

int main() {
    // Experiment knobs (keep simple & consistent)
    constexpr int population_size = 50;
    constexpr int nr_iterations   = 10'000;
    constexpr int dim             = 10;
    constexpr int budget_mult     = 20'000;
    constexpr int budget              = budget_mult * dim;
    const auto out_csv           = "../tmp.csv";

    aha_log::run_aha_and_log_csv_(dim, population_size, nr_iterations, budget, out_csv);

    return 0;
}
