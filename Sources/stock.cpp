// Stock.cpp mock file


#include "../Headers/stock.h"
#include <cstdlib>
#include <cmath>

// Implement basic constructor
Stock::Stock(std::string name, double initialPrice, double volatility)
    : name(name), initialPrice(initialPrice), currentPrice(initialPrice), volatility(volatility) {}

// Mock the 30-day path generation loop your portfolio code calls
std::vector<double> Stock::generatePricePath(int days) {
    std::vector<double> path;
    double price = initialPrice;
    path.push_back(price);
    
    for (int i = 1; i < days; ++i) {
        // Generates a realistic daily price fluctuation based on volatility
        double randFactor = ((double)rand() / RAND_MAX) * 2.0 - 1.0; 
        price += price * (volatility * randFactor * 0.12 + 0.001);
        path.push_back(price);
    }
    return path;
}

// Simple getters matching stock.h declarations
std::string Stock::getName() const { return name; }
double Stock::getInitialPrice() const { return initialPrice; }
double Stock::getCurrentPrice() const { return currentPrice; }
double Stock::getVolatility() const { return volatility; }

// Dummy implementations for virtual placeholders to satisfy compilation
double Stock::simulate() { return initialPrice; }
void Stock::reset() {}
void Stock::printInfo() const {}