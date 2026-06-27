#include "../Headers/portfolio.h"
#include<iostream>
#include<iomanip>
#include<sstream>
#include<numeric>
#include<algorithm>
#include<cmath>
#include<stdexcept>


// Constructor for portfolio class


Portfolio::Portfolio(std::string name, int numSimulations, int numDays)
    : name(name),
    numSimulations(numSimulations),
    numDays(numDays),
    metrics{},
    simulated(false)
{
    portfolioReturns.reserve(numSimulations);
}


void Portfolio::addStock(const std::string& sName, double initialPrice, double vol, double weight) {
    StockEntry entry {Stock(sName, initialPrice, vol), weight};  // Forming Structure
    entries.push_back(entry); // pushing each structure to the vector
}


// Validate the weights

void Portfolio::validateWeights() const {
    double total = 0;
    for (const auto& e: entries) {
        total += e.weight;
    }

    if ( std::abs(total - 1.0) > 0.01) {
        std::cout << "[WARNING] Weights sum to: " << std::fixed << std::setprecision(4) << total << "[Expected: 1.0]. The result may be misleading.\n";
    }
}


// Blended Return = Summation ( Stock Return * Weight )

double Portfolio::blendReturns(const std::vector<double>& simReturns) const {
    double blended = 0;
    for (size_t i = 0; i < entries.size(); i++ ) {
        blended = simReturns[i] * entries[i].weight;
    }
    return blended;
}


void Portfolio::simulatePortfolio() {
    if (entries.empty()) {
        std::cerr << "No stocks added.\n";
        return;
    }

    validateWeights();

    
}