//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_PROBLEM_H
#define SF2565_PROJECT_PROBLEM_H

#include <utility>
#include <vector>
#include <algorithm>
#include <stdexcept>

/*
 * Abstract base class for optimization problems.
 *
 * A Problem defines:
 *   - Dimension d
 *   - Lower/upper bounds for each dimension
 *   - An objective function evaluate(x) that returns the fitness (to be minimized)
 *
 * The class also tracks how many times evaluate(x) has been called (evalCount),
 * which is useful for profiling and fair comparisons between runs.
 */
class Problem {
private:
    std::vector<double> lowerBound;
    std::vector<double> upperBound;
    mutable size_t evalCount = 0; // mutable so evaluate() can stay const

public:
    virtual ~Problem() = default;

    // Store bounds and validate sizes and ordering.
    Problem(std::vector<double> low, std::vector<double> up)
        : lowerBound(std::move(low)), upperBound(std::move(up)) {
        if (lowerBound.size() != upperBound.size() || lowerBound.empty()) {
            throw std::invalid_argument("Bounds must have equal, nonzero size!");
        }
        for (size_t i = 0; i < lowerBound.size(); ++i) {
            if (lowerBound[i] > upperBound[i]) {
                throw std::invalid_argument("Lower bound must be element-wise smaller or equal to upper bound!");
            }
        }
    }

    // Increment evaluation counter (called by concrete evaluate implementations).
    void incrementEvalCount() const { evalCount++; }

    // Objective function: must be implemented by concrete problems.
    [[nodiscard]] virtual double evaluate(const std::vector<double>& x) const = 0;

    [[nodiscard]] size_t dimension() const { return lowerBound.size(); }
    [[nodiscard]] const std::vector<double>& getLowerBound() const { return lowerBound; }
    [[nodiscard]] const std::vector<double>& getUpperBound() const { return upperBound; }
    [[nodiscard]] size_t getEvalCount() const { return evalCount; }
    void resetEvalCount() const { evalCount = 0; }

    // Helper to clamp a candidate solution to the problem bounds.
    [[nodiscard]] std::vector<double> clipToBoundaries(const std::vector<double>& solution) const {
        if (solution.size() != lowerBound.size()) {
            throw std::invalid_argument("Solution size does not match problem dimension.");
        }

        std::vector<double> result(solution.size(), 0.0);
        for (size_t i = 0; i < solution.size(); ++i) {
            result[i] = std::clamp(solution[i], lowerBound[i], upperBound[i]);
        }
        return result;
    }
};

#endif //SF2565_PROJECT_PROBLEM_H
