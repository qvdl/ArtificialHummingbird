#include <iostream>
#include "RNG_Utils.h"
#include "Problems/SphereProblem.h"
#include "Problems/AbsoluteValueProblem.h"
#include "Problems/ShiftedQuadratic.h"
#include "helpers.h"

int main() {
    RNG_Utils::initialize(42);
    const int pop=30, iters=800;

    SphereProblem sphere(10);
    run_and_check_to_zero(sphere, pop, iters, 1e-8, 1e-4, "Sphere(d=10)");

    AbsoluteValueProblem abs10(10);
    run_and_check_to_zero(abs10, pop, iters, 1e-8, 1e-4, "Abs(d=10)");

    ShiftedQuadratic sq;
    run_and_check_to_point(sq, pop, iters, {3.0,-2.0}, 1e-8, 1e-4, "ShiftedQuadratic");

    return 0;
}
