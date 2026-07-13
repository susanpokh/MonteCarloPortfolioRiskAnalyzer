// analyzer.cpp — Implementation of the Analyzer class

#include "analysis.h"

#include <iostream>
#include <iomanip>
#include <numeric>      // std::accumulate
#include <algorithm>    // std::min_element, std::max_element
#include <cmath>        // std::sqrt, std::pow
#include <vector>
#include <string>

// Constructor

Analyzer::Analyzer(const Simulation& sim)
    : sim(sim),
      initPrice(sim.getAllPaths().empty() ? 0.0 : sim.getAllPaths()[0][0]),   // Day 0 of first path
      metrics{},
      computed(false)
{}

//  PRIVATE HELPERS — each one calculates one metric

/*  
    computeExpectedReturn()
    FORMULA:
    return_i = (finalPrice_i - initPrice) / initPrice
    expectedReturn = average of all return_i values 
*/

double Analyzer::computeExpectedReturn() const
{
    const auto& fp = sim.getFinalPrices();
    double sumReturns = 0.0;

    for (double price : fp)
        sumReturns += (price - initPrice) / initPrice;

    return sumReturns / fp.size();
}

/* 
    computeVolatility()
    FORMULA:  standard deviation of all per-simulation returns
    Steps:
    1. Compute every return_i
    2. Compute mean return (= expectedReturn)
    3. For each return_i, square the difference from the mean
    4. Average those squares
    5. Take square root
*/

double Analyzer::computeVolatility() const
{
    const auto& fp = sim.getFinalPrices();

    std::vector<double> returns;
    returns.reserve(fp.size());
    double sumR = 0.0;
    for (double price : fp) {
        double r = (price - initPrice) / initPrice;
        returns.push_back(r);
        sumR += r;
    }
    double meanR = sumR / returns.size();

    double sumSq = 0.0;
    for (double r : returns)
        sumSq += (r - meanR) * (r - meanR);

    double variance = sumSq / returns.size();

    return std::sqrt(variance);
}

/*
    computeLossProbability()

    FORMULA:
    lossProbability = (number of sims where finalPrice < initPrice) / totalSims
*/

double Analyzer::computeLossProbability() const
{
    const auto& fp = sim.getFinalPrices();
    int lossCount = 0;

    for (double price : fp)
        if (price < initPrice) lossCount++;

    return static_cast<double>(lossCount) / fp.size();
}

/*
    computeMaxDrawdown()

    FORMULA:
    For EACH simulation path, find the worst peak-to-trough drop.
    MaxDrawdown = the worst such drop across ALL paths.
    Drawdown = (Lowest Price - Highest Price) / Highest Prive )
*/


double Analyzer::computeMaxDrawdown() const
{
    const auto& paths = sim.getAllPaths();
    double worstDrawdown = 0.0;   // will be negative or zero

    for (const auto& path : paths)
    {
        double peak = path[0];   // start: the initial price is the peak

        for (double price : path)
        {
            // Update peak if today's price is higher
            if (price > peak) peak = price;

            // Drawdown from this peak to today
            double drawdown = (price - peak) / peak;

            // Keep track of the worst (most negative) drawdown
            if (drawdown < worstDrawdown)
                worstDrawdown = drawdown;
        }
    }

    return worstDrawdown;   
}

/*
    computeWorstCase()
    The single lowest return seen across all simulations.
*/

double Analyzer::computeWorstCase() const
{
    const auto& fp = sim.getFinalPrices();
    double worstPrice = *std::min_element(fp.begin(), fp.end());
    return (worstPrice - initPrice) / initPrice;
}

// computeBestCase()

double Analyzer::computeBestCase() const
{
    const auto& fp = sim.getFinalPrices();
    double bestPrice = *std::max_element(fp.begin(), fp.end());
    return (bestPrice - initPrice) / initPrice;
}

// computeAvgFinalPrice()

double Analyzer::computeAvgFinalPrice() const
{
    const auto& fp = sim.getFinalPrices();
    double sum = std::accumulate(fp.begin(), fp.end(), 0.0);
    return sum / fp.size();
}

//  PUBLIC INTERFACE

//compute() — run all helpers and cache results in `metrics`

void Analyzer::compute()
{
    if (sim.getFinalPrices().empty()) {
        std::cerr << "[Analyzer] No simulation data. " << "Run Simulation::runMonteCarlo() first.\n";
        return;
    }

    metrics.expectedReturn = computeExpectedReturn();
    metrics.volatility = computeVolatility();
    metrics.lossProbability = computeLossProbability();
    metrics.maxDrawdown = computeMaxDrawdown();
    metrics.worstCase = computeWorstCase();
    metrics.bestCase = computeBestCase();
    metrics.avgFinalPrice = computeAvgFinalPrice();

    computed = true;
}

// Getters

double Analyzer::getExpectedReturn()const { 
    return metrics.expectedReturn;  
    }
double Analyzer::getVolatility() const{
    return metrics.volatility;
    }
double Analyzer::getLossProbability() const{
    return metrics.lossProbability;
    }
double Analyzer::getMaxDrawdown() const{
    return metrics.maxDrawdown;
    }
double Analyzer::getWorstCase() const{
    return metrics.worstCase;
    }
double Analyzer::getBestCase() const{
    return metrics.bestCase;
    }
double Analyzer::getAvgFinalPrice() const{
    return metrics.avgFinalPrice;
    }

RiskMetrics Analyzer::getMetrics() const{
    return metrics;
    }

// Print Report

void Analyzer::printReport() const
{
    if (!computed) {
        std::cout << "Please call compute() first before printReport().\n";
        return;
    }

    // figure out risk level from volatility
    std::string riskLabel;
    if (metrics.volatility < 0.08)
        riskLabel = "LOW";
    else if (metrics.volatility < 0.15)
        riskLabel = "MEDIUM";
    else
        riskLabel = "HIGH";

    // print out the report
    std::cout << "\n";
    std::cout << "==============================================\n";
    std::cout << "           RISK ANALYSIS REPORT\n";
    std::cout << "==============================================\n";

    std::cout << "Simulations: " << sim.getNumSimulations()
              << " (" << sim.getNumDays() << " days each)\n\n";

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "RETURNS\n";
    std::cout << "  Expected Return   : " << metrics.expectedReturn * 100 << "%\n";
    std::cout << "  Worst Case Return : " << metrics.worstCase * 100 << "%\n";
    std::cout << "  Best Case Return  : " << metrics.bestCase * 100 << "%\n\n";

    std::cout << "RISK METRICS\n";
    std::cout << "  Volatility       : " << std::setprecision(4) << metrics.volatility << "\n";
    std::cout << std::setprecision(2);
    std::cout << "  Loss Probability : " << metrics.lossProbability * 100 << "%\n";
    std::cout << "  Max Drawdown     : " << metrics.maxDrawdown * 100 << "%\n\n";

    std::cout << "PRICE\n";
    std::cout << "  Avg Final Price  : $" << metrics.avgFinalPrice << "\n\n";

    std::cout << "Risk Level: " << riskLabel << "\n";
    std::cout << "==============================================\n";


    std::cout << "\n--- ML Input Features (for API in Step 12) ---\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  expected_return : " << metrics.expectedReturn  << "\n";
    std::cout << "  volatility      : " << metrics.volatility      << "\n";
    std::cout << "  loss_prob       : " << metrics.lossProbability  << "\n";
    std::cout << "  drawdown        : " << metrics.maxDrawdown      << "\n";
};
