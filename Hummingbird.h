#ifndef SF2565_PROJECT_HUMMINGBIRD_H
#define SF2565_PROJECT_HUMMINGBIRD_H

#include "Problem.h"
#include "RNG_Utils.h"
#include <vector>

/*
 * Each hummingbird:
 *   - Holds a position vector (candidate solution x)
 *   - Stores the corresponding fitness f(x)
 *
 * The struct is intentionally lightweight, as many instances are created
 * and updated during optimization.
 */
struct Hummingbird {
    std::vector<double> position;
    double fitness = 0.0;

    Hummingbird() = default;

    // Initialize a hummingbird at a random position within the problem bounds
    // and immediately evaluate its fitness.
    explicit Hummingbird(const Problem& prob) {
        position.resize(prob.getLowerBound().size());
        const auto& low = prob.getLowerBound();
        const auto& upp = prob.getUpperBound();

        for (size_t i = 0; i < position.size(); ++i) {
            const double r = RNG_Utils::random01();
            position[i] = low[i] + r * (upp[i] - low[i]);
        }

        fitness = prob.evaluate(position);
    }

    // Accessors used by the optimizer and visit table logic.
    [[nodiscard]] std::vector<double> getPosition() const { return position; }
    [[nodiscard]] double getFitness() const { return fitness; }
};

#endif //SF2565_PROJECT_HUMMINGBIRD_H
