//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_PROBLEM_H
#define SF2565_PROJECT_PROBLEM_H

#include <utility>
#include <vector>
#include <algorithm>
#include <stdexcept>

class Problem {
private:
    std::vector<double> lowerBound;
    std::vector<double> upperBound;
public:
    virtual ~Problem() = default;

    Problem(std::vector<double> low, std::vector<double> up) : lowerBound(std::move(low)), upperBound(std::move(up)) {
        if (lowerBound.size() != upperBound.size() || lowerBound.empty()) {
            throw std::invalid_argument("Bounds must have equal, nonzero size!");
        }
        for (size_t i=0; i<lowerBound.size(); ++i) {
            if (lowerBound[i] > upperBound[i]) {
                throw std::invalid_argument("Lower bound must be element-wise smaller or equal to upper bound!");
            }
        }
    }

    [[nodiscard]] virtual double evaluate(const std::vector<double>& x) const = 0;

    [[nodiscard]] size_t dimension() const { return lowerBound.size(); }
    [[nodiscard]] const std::vector<double>& getLowerBound() const { return lowerBound; }
    [[nodiscard]] const std::vector<double>& getUpperBound() const { return upperBound; }

    [[nodiscard]] std::vector<double> clipToBoundaries(const std::vector<double>& solution) const {
        if (solution.size() != lowerBound.size()) {
            throw std::invalid_argument("Solution size does not match problem dimension.");
        }
        std::vector<double> result(solution.size(), 0.0);
        for (size_t i=0; i<solution.size(); ++i) {
            result[i] = std::clamp(solution[i], lowerBound[i], upperBound[i]);
        }

        return result;
    }
};


#endif //SF2565_PROJECT_PROBLEM_H