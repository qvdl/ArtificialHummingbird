//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_FLIGHTENGINE_H
#define SF2565_PROJECT_FLIGHTENGINE_H

#include <vector>

/*
 * FlightEngine generates binary masks D used in the AHA update equations.
 * A mask entry D[k] = 1 means "update along dimension k", otherwise keep it fixed.
 *
 * The three flight types correspond to Eqs. (3)–(5) in the paper:
 *   - axial:  update one axis
 *   - diagonal: update a random subset of axes
 *   - omni:   update all axes
 */

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