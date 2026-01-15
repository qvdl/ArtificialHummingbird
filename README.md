Artificial Hummingbird Algorithm (AHA) – C++ Implementation

This project contains a C++ implementation of the Artificial Hummingbird Algorithm (AHA), a bio-inspired metaheuristic optimizer proposed by Zhao et al. The implementation focuses on translating the algorithm into efficient, readable C++ code and analyzing its performance on standard benchmark functions.

Structure

AHA_Optimizer – Core implementation of the AHA optimization loop

Hummingbird – Represents a candidate solution and its fitness

VisitTable – Stores visit history used for guided foraging

FlightEngine – Generates axial, diagonal, and omnidirectional flight masks

Problem – Abstract base class for optimization problems

Problems/ – Collection of benchmark objective functions

RNG_Utils – Centralized random number generation utilities

main.cpp – Example entry point to run AHA on a selected problem

Building and Running

Compile with a C++20-compatible compiler. For example:

g++ -O3 -std=c++20 *.cpp Problems/*.cpp -o aha


Run the program:

./aha


To change the optimization problem, edit main.cpp and replace the selected Problem subclass.

Notes

Randomness is fully reproducible via a fixed seed.

The implementation prioritizes low overhead in the main optimization loop.

Benchmarking and profiling code is included separately from the demo main.

Reference

Zhao, W., Wang, L., & Mirjalili, S.
Artificial hummingbird algorithm: A new bio-inspired optimizer.
Engineering Applications of Artificial Intelligence, 114, 114194 (2022).