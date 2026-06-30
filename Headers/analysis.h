#ifndef ANALYSIS_H
#define ANALYSIS_H

/*
METRICS
  expectedReturn   — average % gain/loss across all sims
  volatility       — how spread out the outcomes are (std dev)
  lossProbability  — fraction of sims that lost money
  maxDrawdown      — worst peak-to-trough drop in any path
  worstCase        — lowest return seen across all sims
  bestCase         — highest return seen across all sims
  avgFinalPrice    — mean final price across all sims
*/

#include "simulation.h"
#include <vector>

// Structure to store all the risk metrics

struct RiskMetrics {
    double expectedReturn;   
    double volatility;        
    double lossProbability;   
    double maxDrawdown;       
    double worstCase;        
    double bestCase;         
    double avgFinalPrice;     
};


class Analyzer {
private:
    const Simulation& sim;
    double            initPrice;

    RiskMetrics metrics;
    bool        computed; 

    double computeExpectedReturn()  const;
    double computeVolatility()      const;
    double computeLossProbability() const;
    double computeMaxDrawdown()     const;
    double computeWorstCase()       const;
    double computeBestCase()        const;
    double computeAvgFinalPrice()   const;

public:

    explicit Analyzer(const Simulation& sim);

    void compute();

    // getter methods
    double getExpectedReturn()   const;
    double getVolatility()       const;
    double getLossProbability()  const;
    double getMaxDrawdown()      const;
    double getWorstCase()        const;
    double getBestCase()         const;
    double getAvgFinalPrice()    const;

    // get all risk metrics together
    RiskMetrics getMetrics() const;

    void printReport() const;
};

#endif
