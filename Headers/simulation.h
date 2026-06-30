#ifndef SIMULATION_H
#define SIMULATION_H



#include "stock.h"
#include <vector>

class Simulation {
private:
    Stock&  stock;

    int     numSimulations;   // how many runs  (example 1000)
    int     numDays;          // how many days  (example 30)

    // finalPrices  = the closing price on Day numDays of run i
    // allPaths     = the full day by day path of run i
    std::vector<double>              finalPrices;
    std::vector<std::vector<double>> allPaths;

public:
    // Constructor
    Simulation(Stock& stock, int numSimulations = 1000, int numDays = 30);


    // Runs all simulations and fills finalPrices and allPaths
    void runMonteCarlo();

    const std::vector<double>&              getFinalPrices() const;
    const std::vector<std::vector<double>>& getAllPaths()    const;
    int                                     getNumSimulations() const;
    int                                     getNumDays()        const;

    void printSummary() const;
};

#endif 
