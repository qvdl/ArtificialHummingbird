# Artificial Hummingbird Algorithm (AHA) – C++ Implementation

This project contains a C++ implementation of the **Artificial Hummingbird Algorithm (AHA)**,  
a bio-inspired metaheuristic optimizer proposed by Zhao et al.  
The focus of the project is on translating the algorithm into efficient, readable C++ code and analyzing its behavior on benchmark optimization problems.

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
g++ -O3 -std=c++20 *.cpp Problems/*.cpp -o aha
```

## Running

```bash
./aha
```

## Notes

To change the optimization problem, edit main.cpp and replace the selected Problem subclass.
Randomness is fully reproducible via a fixed seed.
Benchmarking code is included in benchmark_driver.cpp

## Reference

Zhao, W., Wang, L., & Mirjalili, S. Artificial hummingbird algorithm: A new bio-inspired optimizer. Engineering Applications of Artificial Intelligence, 114, 114194 (2022).