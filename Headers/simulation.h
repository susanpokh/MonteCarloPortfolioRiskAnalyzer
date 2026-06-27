#ifndef SIMULATION_H
#define SIMULATION_H

// ============================================================
// simulation.h — Declaration of the Simulation class
// STEP 3: Runs the Monte Carlo loop over a single Stock
// ============================================================

#include "stock.h"
#include <vector>

class Simulation {
private:
    // The stock we are simulating (stored by reference so we
    // don't copy it — the same Stock object is used throughout)
    Stock&  stock;

    int     numSimulations;   // how many independent runs  (e.g. 1000)
    int     numDays;          // how many days per run      (e.g. 30)

    // Results storage
    // finalPrices[i]  = the closing price on Day numDays of run i
    // allPaths[i]     = the full day-by-day path of run i
    std::vector<double>              finalPrices;
    std::vector<std::vector<double>> allPaths;

public:
    // Constructor
    Simulation(Stock& stock, int numSimulations = 1000, int numDays = 30);

    // ── Core engine ──────────────────────────────────────────
    // Runs all simulations and fills finalPrices + allPaths
    void runMonteCarlo();

    // ── Accessors ────────────────────────────────────────────
    const std::vector<double>&              getFinalPrices() const;
    const std::vector<std::vector<double>>& getAllPaths()    const;
    int                                     getNumSimulations() const;
    int                                     getNumDays()        const;

    // ── Quick summary (prints to console) ────────────────────
    void printSummary() const;
};

#endif // SIMULATION_H
