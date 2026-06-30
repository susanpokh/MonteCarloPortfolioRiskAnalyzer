#ifndef STOCK_H
#define STOCK_H


#include <string>
#include <vector> 

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

    double simulate();

    // Step 2 — NEW: simulate 'days' steps, return every price
    // Default argument: if we call generatePricePath() with no argument, if nothing is inputted we'll simply input 30
    std::vector<double> generatePricePath(int days = 30);

    // Reset price back to initialPrice
    void reset();

    // Getter methods
    std::string getName() const;
    double getInitialPrice() const;
    double getCurrentPrice() const;
    double getVolatility() const;

    void printInfo() const;
};

#endif
