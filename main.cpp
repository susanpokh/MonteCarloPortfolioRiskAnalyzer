// main.cpp mock file


#include "Headers/portfolio.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Seed random generation for realistic mock simulation paths
    std::srand(std::time(0));

    // std::cout << "====================================================\n";
    // std::cout << "     INITIALIZING PORTFOLIO MODULE VERIFICATION    \n";
    // std::cout << "====================================================\n\n";

    // Create a portfolio targeting a 1000-run, 30-day Monte Carlo block
    Portfolio corporatePortfolio("Aggressive Tech Allocation", 1000, 30);

    // Add stocks using your exact addStock function signatures
    corporatePortfolio.addStock("AAPL", 175.00, 0.18, 0.40); // 40% Weight
    corporatePortfolio.addStock("NVDA", 480.00, 0.45, 0.35); // 35% Weight
    corporatePortfolio.addStock("MSFT", 400.00, 0.12, 0.25); // 25% Weight

    // Display asset holdings structural grid
    corporatePortfolio.printHoldings();

    // Fire your core engine simulation algorithms
    corporatePortfolio.simulatePortfolio();

    // Output statistical calculations matrix and histogram distribution charts
    corporatePortfolio.printReport();

    return 0;
}