#ifndef STOCK_H
#define STOCK_H

// ============================================================
// stock.h — Declaration of the Stock class
// STEP 1: Basic stock with one-step price simulation
// STEP 2: Added generatePricePath() — returns full N-day path
// ============================================================

#include <string>
#include <vector> // NEW in Step 2

class Stock
{
private:
    std::string name;
    double initialPrice;
    double currentPrice;
    double volatility;

public:
    // Constructor
    Stock(std::string name, double initialPrice, double volatility);

    // Step 1 — single price step (unchanged)
    double simulate();

    // Step 2 — NEW: simulate 'days' steps, return every price
    // Default argument: if you call generatePricePath() with no
    // argument, it automatically uses 30 days.
    std::vector<double> generatePricePath(int days = 30);

    // Reset price back to initialPrice
    void reset();

    // Getters
    std::string getName() const;
    double getInitialPrice() const;
    double getCurrentPrice() const;
    double getVolatility() const;

    // Display helper
    void printInfo() const;
};

#endif // STOCK_H
