//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_HUMMINGBIRD_H
#define SF2565_PROJECT_HUMMINGBIRD_H

#include "Problem.h"
#include "RNG_Utils.h"
#include <vector>


struct Hummingbird {
    std::vector<double> position;
    double fitness=0.0;

    Hummingbird() = default;
    explicit Hummingbird(const Problem& prob) {
        position.resize(prob.getLowerBound().size());
        const auto& low = prob.getLowerBound();
        const auto& upp = prob.getUpperBound();

        for (size_t i=0; i<position.size(); ++i) {
            const double r = RNG_Utils::random01();
            position[i] = low[i] + r * (upp[i] - low[i]);
        }

        fitness = prob.evaluate(position);
    }

    [[nodiscard]] std::vector<double> getPosition() const { return position; }
    [[nodiscard]] double getFitness() const { return fitness; }
};

#endif //SF2565_PROJECT_HUMMINGBIRD_H