#include "portfolio.h"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <cmath>


// Constructor of Portfolio class

Portfolio::Portfolio(std::string name, int numSimulations, int numDays)
    : name(name), // Name of individual portfolio. Can be anything
      numSimulations(numSimulations), // Number of simulations to do
      numDays(numDays), // Number of days the simulation will run
      metrics{}, // Empty structure member, with empty values at first. After finding the riskMetrics we are going to assign the risk metrics
      simulated(false) // Check if the simulation is done or not, (As a default it is set to false since no any simulations is done yet)
{
    portfolioReturns.reserve(numSimulations); // if numSimulation runs for n times then we know, portfolioReturns vector will have n elements. So, we allocate the size of the vector in advance which can help prevent resizing overload.
}

void Portfolio::addStock(const std::string &sName, double initialPrice, double vol, double weight) // We are goig to add a given stock with it's weight (All things are inputted by the user)
{
    StockEntry entry{Stock(sName, initialPrice, vol), weight}; // Forming Structure StockEntry which consists of one object which is Stock, and the other elment which is weight of the portfolio (Inputted by the user)
    entries.push_back(entry); // pushing each structure entry to the entries vector
}

// entries[0] -> Stock(Name, InitialPrice, Volatility), Weight
// entires[1] -> Stock(Name, InitialPrice, Volatiltiy), Weight and so onnnn

void Portfolio::validateWeights() // Here, with this function, we are going to check if the weights inputted by the user add upto 100%. It's because user needs to invest his whole money, and of couse the weights of his investment is always 100%
{
    double total = 0;
    for (const auto &e : entries)
    {
        total += e.weight; // We are taking the weights of each stock and adding them up
    }

    if (std::abs(total - 1.0) > 0.01) // Checking if the total weight add upto 100, if not, we will simply deliver a warning message
    {
        std::cout << "[WARNING] Weights sum to: " << std::fixed << std::setprecision(4) << total << "[Expected: 1.0]. The result may be misleading.\n";
    }
}


// Blended Return = Summation ( Stock Return * Weight )

double Portfolio::blendReturns(const std::vector<double> &simReturns)
{
    double blended = 0;
    for (size_t i = 0; i < entries.size(); i++)
    {
        blended += simReturns[i] * entries[i].weight;
    }
    return blended;
}

void Portfolio::simulatePortfolio()
{
    if (entries.empty())
    {
        std::cout << "No stocks added.\n";
        return;
    }

    validateWeights();

    portfolioReturns.clear();
    portfolioReturns.reserve(numSimulations);

    std::cout << "SImulating Portfolio \"" << name << "\" (" << entries.size() << " stocks, " << numSimulations << " runs)";

    for (int run = 0; run < numSimulations; run++)
    {

        if (run % 100 == 0)
        {
            std::cout << ".";

        }

        std::vector<double> runReturns;
        runReturns.reserve(entries.size());

        for (auto &entry : entries)
        {
            std::vector<double> path = entry.stock.generatePricePath(numDays);
            double initP = entry.stock.getInitialPrice();
            double finalP = path.back();
            double ret = (finalP - initP) / initP;
            runReturns.push_back(ret);
        }

        double portfolioRet = blendReturns(runReturns);
        portfolioReturns.push_back(portfolioRet);
    }

    std::cout << "Finished!\n";

    // Expected Return
    double sumR = std::accumulate(portfolioReturns.begin(), portfolioReturns.end(), 0);
    metrics.expectedReturn = sumR / portfolioReturns.size();

    // Volatility
    double sumSq = 0;
    for (double r : portfolioReturns)
    {
        sumSq += (r - metrics.expectedReturn) * (r - metrics.expectedReturn);
    }
    metrics.volatility = std::sqrt(sumSq / portfolioReturns.size());

    // Loss Probability
    int lossCount = 0;
    for (double r : portfolioReturns)
    {
        if (r < 0)
            lossCount++;
    }
    metrics.lossProbability = static_cast<double>(lossCount) / portfolioReturns.size();

    // Worst / best case
    metrics.worstCase = *std::min_element(portfolioReturns.begin(), portfolioReturns.end());
    metrics.bestCase = *std::max_element(portfolioReturns.begin(), portfolioReturns.end());

    // Maximum Drawdown
    double peak = 0, worstDD = 0;
    double cumulative = 0;
    for (double r : portfolioReturns)
    {
        cumulative += r / portfolioReturns.size(); // mean
        if (cumulative > peak)
            peak = cumulative;
        double dd = (peak > 0) ? (cumulative - peak) / peak : 0;
        if (dd < worstDD)
            worstDD = dd;
    }
    metrics.maxDrawdown = metrics.worstCase;
    metrics.avgFinalPrice = 1 + metrics.expectedReturn;

    simulated = true;
}




void Portfolio::printHoldings()
{
    std::cout << "\n--- CURRENT PORTFOLIO HOLDINGS ---\n";
    for (const auto &e : entries)
    {
        std::cout << "Asset: " << e.stock.getName()
                  << " | Price: " << e.stock.getInitialPrice()
                  << " | Volatility: " << e.stock.getVolatility()
                  << " | Weight: " << e.weight << "\n";
    }
    std::cout << "--------------------------------------\n";
}


void Portfolio::printReport()
{
    if (!simulated)
    {
        std::cout << "Error: Run simulatePortfolio first.\n";
        return;
    }

    std::cout << "\n--- SIMULATION RISK METRICS RESULTS ---\n";
    std::cout << "Portfolio Name: " << name << "\n";
    std::cout << "Expected Return: " << metrics.expectedReturn << "\n";
    std::cout << "Volatility: " << metrics.volatility << "\n";
    std::cout << "Loss Probability: " << metrics.lossProbability << "\n";
    std::cout << "Worst Case Return: " << metrics.worstCase << "\n";
    std::cout << "Best Case Return: " << metrics.bestCase << "\n";
    std::cout << "Max Drawdown Proxy: " << metrics.maxDrawdown << "\n";
    std::cout << "Normalized Avg Final Value: " << metrics.avgFinalPrice << "\n";
    std::cout << "-------------------------------------------\n";
}


std::vector<double> Portfolio::getPortfolioReturns() { return portfolioReturns; }
RiskMetrics Portfolio::getMetrics() { return metrics; }
std::string Portfolio::getName() { return name; }
int Portfolio::getStockCount() { return entries.size(); }