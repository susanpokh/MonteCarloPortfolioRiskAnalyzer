#ifndef PORTFOLIO_H
#define PORTFOLIO_H

// ============================================================
// portfolio.h — Declaration of the Portfolio class
// STEP 5: Holds multiple stocks + weights, runs combined MC
//
// WHAT IS A PORTFOLIO?
//   A portfolio is a collection of stocks where each stock
//   gets a "weight" — the fraction of total money invested.
//
//   Example: $10,000 total investment
//     AAPL  40%  →  $4,000
//     TSLA  35%  →  $3,500
//     MSFT  25%  →  $2,500
//     Total: 100% = $10,000
//
//   Portfolio return = sum of (stock_return * stock_weight)
//   So if AAPL returns +10%, TSLA -5%, MSFT +8%:
//   Portfolio return = (0.10 × 0.40) + (-0.05 × 0.35) + (0.08 × 0.25)
//                    = 0.04 - 0.0175 + 0.02
//                    = 0.0425   (+4.25%)
// ============================================================

#include "stock.h"
#include "analysis.h"
#include <vector>
#include <string>

// ── A struct that pairs a Stock with its portfolio weight ─────
struct StockEntry {
    Stock  stock;    // the Stock object (name, price, volatility)
    double weight;   // fraction of portfolio, e.g. 0.40 = 40%
};

// ── The Portfolio class ───────────────────────────────────────
class Portfolio {
private:
    std::string             name;          // e.g. "Tech Portfolio"
    std::vector<StockEntry> entries;       // all stocks + their weights
    int                     numSimulations;
    int                     numDays;

    // Combined simulation results
    // portfolioReturns[i] = the blended return of simulation run i
    std::vector<double> portfolioReturns;

    // Cached risk metrics (filled by simulatePortfolio)
    RiskMetrics metrics;
    bool        simulated;

    // ── Private helpers ───────────────────────────────────────
    void  validateWeights()   const;   // warns if weights don't sum to 1
    double blendReturns(const std::vector<double>& simReturns) const;

public:
    // Constructor
    Portfolio(std::string name,
              int numSimulations = 1000,
              int numDays        = 30);

    // ── Build the portfolio ───────────────────────────────────
    // Add a stock with a given weight (0.0 – 1.0)
    void addStock(const std::string& name,
                  double initialPrice,
                  double volatility,
                  double weight);

    // ── Run simulation ────────────────────────────────────────
    // Simulates every stock, blends returns, computes metrics
    void simulatePortfolio();

    // ── Accessors ─────────────────────────────────────────────
    const std::vector<double>& getPortfolioReturns() const;
    RiskMetrics                getMetrics()          const;
    std::string                getName()             const;
    int                        getStockCount()       const;

    // ── Display ───────────────────────────────────────────────
    void printHoldings()  const;   // show stocks + weights
    void printReport()    const;   // full risk report
};

#endif // PORTFOLIO_H
