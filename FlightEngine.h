//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_FLIGHTENGINE_H
#define SF2565_PROJECT_FLIGHTENGINE_H

#include <vector>

class FlightEngine {
private:
    int dimension;

public:
    explicit FlightEngine(const int d) : dimension(d) {}

    [[nodiscard]] std::vector<int> axialFlight() const;
    [[nodiscard]] std::vector<int> diagonalFlight() const;
    [[nodiscard]] std::vector<int> omniFlight() const;
};

#endif //SF2565_PROJECT_FLIGHTENGINE_H