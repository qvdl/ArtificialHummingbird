# Artificial Hummingbird Algorithm (AHA) – C++ Implementation

This project contains a C++ implementation of the **Artificial Hummingbird Algorithm (AHA)**,  
a bio-inspired metaheuristic optimizer proposed by Zhao et al.  

## Structure

- `AHA_Optimizer` – Core implementation of the AHA optimization loop
- `Hummingbird` – Represents a candidate solution and its fitness
- `VisitTable` – Stores visit history used for guided foraging
- `FlightEngine` – Generates axial, diagonal, and omnidirectional flight masks
- `Problem` – Abstract base class for optimization problems
- `Problems/` – Collection of benchmark objective functions
- `RNG_Utils` – Random number generation utilities
- `main.cpp` – Example entry point to run AHA on a selected problem

## Building

Compile with a C++20-compatible compiler. For example:

```bash
g++ -std=gnu++20 *.cpp -o aha
```

## Running

```bash
./aha
```

## Notes

To change the optimization problem, edit main.cpp and replace the selected Problem subclass.
Randomness is fully reproducible via a fixed seed.
Benchmarking code is included in benchmark_driver.cpp (build with g++ -std=gnu++20 -DAHA_BENCH *.cpp -o aha_bench), which runs the AHA optimizer multiple times on a suite of standard benchmark functions, computes summary statistics over independent runs, optionally compares them against reference results from the original paper, and writes the results to a CSV file for analysis.

## Reference

Zhao, W., Wang, L., & Mirjalili, S. Artificial hummingbird algorithm: A new bio-inspired optimizer. Engineering Applications of Artificial Intelligence, 114, 114194 (2022).