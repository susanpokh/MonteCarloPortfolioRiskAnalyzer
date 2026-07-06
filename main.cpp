// main.cpp mock file

#include "Headers/stock.h"
#include "Headers/simulation.h"
#include "Headers/analysis.h"
#include "Headers/recommendation.h"
#include "Headers/portfolio.h"

#include <iostream>
#include <cstdlib>
#include <ctime>



int main()
{
    // see random number based on computer time calendar
    std::srand(std::time(0));

    // Create a portfolio targeting a 1000 runs, 30 days monte carlo simulation
    Portfolio corporatePortfolio("Aggressive Tech Allocation", 1000, 30);

    corporatePortfolio.addStock("AAPL", 175.00, 0.18, 0.40);
    corporatePortfolio.addStock("NVDA", 480.00, 0.45, 0.35);
    corporatePortfolio.addStock("MSFT", 400.00, 0.12, 0.25);

    corporatePortfolio.printHoldings();

    corporatePortfolio.simulatePortfolio();

    corporatePortfolio.printReport();


    // 1. Teammate's module: define the stock (price, drift, volatility...)
    Stock stock(/* symbol, initial price, params */);

    // 2. Teammate's module: run the Monte Carlo simulation
    Simulation sim(/* stock, numDays, numSimulations */);
    sim.runMonteCarlo();

    // 3. MY module: analyze the raw paths into risk metrics
    Analyzer analyzer(sim);   // must construct AFTER runMonteCarlo(),
                              // because initPrice is read in the constructor
    analyzer.compute();
    analyzer.printReport();

    // 4. Teammate's module: feed metrics to the recommendation engine
    RiskMetrics m = analyzer.getMetrics();
    Recommendation rec(/* m or individual fields */);
    rec.generate();

    return 0;
}
