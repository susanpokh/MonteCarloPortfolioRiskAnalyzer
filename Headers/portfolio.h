#ifndef PORTFOLIO_H
#define PORTFOLIO_H

// We are going to hold multiple stocks, their weights, and run combined monte carlo simulation
// Here a user needs to invest all his/her money to the stocks (All-in portfolio, concentrated portfolio)
// Portfolio is simply a collection of stocks where we assign each stock their weights and sum of each weight should add upto 100%
//   Example: $10,000 total investment
//     AAPL  40%  →  $4,000
//     TSLA  35%  →  $3,500
//     MSFT  25%  →  $2,500
//     Total: 100% = $10,000
//   Portfolio return = summation of (stock_return * stock_weight)
//   if AAPL returns +10%, TSLA -5%, MSFT +8%:
//   Portfolio return = (0.10 × 0.40) + (-0.05 × 0.35) + (0.08 × 0.25)
//                    = 0.04 - 0.0175 + 0.02
//                    = 0.0425 [Equivalent to (+4.25%)]


#include "stock.h"
#include "analysis.h"
#include <vector>
#include <string>

// Using structure to pair each stock data to their individual weights
struct StockEntry {
    Stock  stock;    // Stock object (name, price, volatility)
    double weight;   // stock weight
};



class Portfolio {
private:
    std::string             name;
    std::vector<StockEntry> entries;       // all stocks and their weights
    int                     numSimulations;
    int                     numDays;


    // (Combined Portfolio Returns)
    std::vector<double> portfolioReturns;

    // Risk Metrics
    RiskMetrics metrics;
    bool        simulated; // We'll check if we have done Monte Carlo simulation for combined portfolio


    void  validateWeights()   const;   // we will simply warn the user if the sum of weights is not equal to 1 (100%)
    double blendReturns(const std::vector<double>& simReturns) const;

public:
    // Constructor
    Portfolio(std::string name,
              int numSimulations = 1000,
              int numDays        = 30);

    // Build the portfolio
    // Add a stock with a given weight
    void addStock(const std::string& name,
                  double initialPrice,
                  double volatility,
                  double weight);

    // Run simulation
    // Simulates every stock, blends returns, computes metrics
    void simulatePortfolio();


    const std::vector<double>& getPortfolioReturns() const;
    RiskMetrics                getMetrics()          const;
    std::string                getName()             const;
    int                        getStockCount()       const;

    void printHoldings()  const;   // show stocks and weights
    void printReport()    const;   // full risk report
};

#endif
