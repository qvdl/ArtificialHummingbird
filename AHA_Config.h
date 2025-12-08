//
// Created by quinten on 12/2/25.
//

#ifndef SF2565_PROJECT_AHA_CONFIG_H
#define SF2565_PROJECT_AHA_CONFIG_H


struct AHA_Config {
    int population_size;
    int max_iterations;
    int dimension;
    int migration_coefficient;

    AHA_Config(const int n, const int it, const int d) : population_size(n), max_iterations(it), dimension(d), migration_coefficient(2*n) {}
    AHA_Config(const int n, const int it, const int d, const int m) : population_size(n), max_iterations(it), dimension(d), migration_coefficient(m) {}
};


#endif //SF2565_PROJECT_AHA_CONFIG_H