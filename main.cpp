// main.cpp mock file

#include "Headers/portfolio.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    // Seed random generation for realistic mock simulation paths
    std::srand(std::time(0));

    // Create a portfolio targeting a 1000 runs, 30 days monte carlo simulation
    Portfolio corporatePortfolio("Aggressive Tech Allocation", 1000, 30);

    corporatePortfolio.addStock("AAPL", 175.00, 0.18, 0.40);
    corporatePortfolio.addStock("NVDA", 480.00, 0.45, 0.35);
    corporatePortfolio.addStock("MSFT", 400.00, 0.12, 0.25);

    corporatePortfolio.printHoldings();

    corporatePortfolio.simulatePortfolio();

    corporatePortfolio.printReport();

    return 0;
}