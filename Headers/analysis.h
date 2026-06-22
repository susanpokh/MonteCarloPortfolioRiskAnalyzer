#ifndef ANALYZER_H
#define ANALYZER_H

#include "simulation.h"
#include <vector>

/* 
=================================================================
Module Owner: Manav 
Responsibility: Turn raw Monte Carlo output into meaningful risk/return statistics.
=================================================================
*/

struct RiskMetrics {
    double expectedReturn;   // mean simulated return across all paths
    double volatility;       // standard deviation of returns
    double maxDrawdown;      // largest peak-to-trough decline observed
    double lossProbability;  // fraction of simulations ending below start price
};

class Analyzer {
private:
    const simulation& simulationRef;  // reference to Phuyal's completed simulation

public:
    // Constructor: binds to an already-run Simulation.
    // Analyzer does not own or mutate simulation data, only reads it.
    explicit Analyzer(const simulation& simulationRef);

    // Each function reads from simulation.getAllPaths() / getFinalPrices()

    // Mean percentage return across all simulated final prices vs. start price.
    double calculateExpectedReturn() const;

    // Standard deviation of the final returns (risk/dispersion measure).
    double calculateVolatility() const;

    // Largest observed peak-to-trough drop, averaged or worst-case
    // across all simulated paths (exact aggregation to be agreed on
    // with the team before .cpp implementation).
    double calculateMaxDrawdown() const;

    // Probability (0.0 - 1.0) that a simulation ends with a loss
    // (final price < start price).
    double calculateLossProbability() const;

    // Runs all four calculations above and packages them together.
    RiskMetrics getRiskMetrics() const;
};

#endif