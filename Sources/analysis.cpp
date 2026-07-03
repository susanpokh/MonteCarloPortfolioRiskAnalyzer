// analyzer.cpp — Implementation of the Analyzer class

#include "analyzer.h"

#include <iostream>
#include <iomanip>
#include <numeric>      // std::accumulate
#include <algorithm>    // std::min_element, std::max_element
#include <cmath>        // std::sqrt, std::pow
#include <sstream>      // std::ostringstream
#include <vector>
#include <string>

// Constructor

Analyzer::Analyzer(const Simulation& sim)
    : sim(sim),
      initPrice(sim.getAllPaths().empty()
                ? 0.0
                : sim.getAllPaths()[0][0]),   // Day 0 of first path
      metrics{},
      computed(false)
{}

// ==================================================================
//  PRIVATE HELPERS — each one calculates one metric
// ==================================================================

// ------------------------------------------------------------------
// computeExpectedReturn()
// ------------------------------------------------------------------
// FORMULA:
//   return_i      = (finalPrice_i - initPrice) / initPrice
//   expectedReturn = average of all return_i values
//
// Example with 3 sims:
//   run1: $150 → $163  return = +0.087
//   run2: $150 → $141  return = -0.060
//   run3: $150 → $155  return = +0.033
//   expected = (0.087 - 0.060 + 0.033) / 3 = +0.020  (+2%)
// ------------------------------------------------------------------
double Analyzer::computeExpectedReturn() const
{
    const auto& fp = sim.getFinalPrices();
    double sumReturns = 0.0;

    for (double price : fp)
        sumReturns += (price - initPrice) / initPrice;

    return sumReturns / fp.size();
}

// ------------------------------------------------------------------
// computeVolatility()
// ------------------------------------------------------------------
// FORMULA:  standard deviation of all per-simulation returns
//
// Standard deviation measures SPREAD — how far outcomes stray
// from the average.  High std dev = high risk (wide range of
// possible outcomes).  Low std dev = outcomes cluster tightly.
//
// Steps:
//   1. Compute every return_i
//   2. Compute mean return (= expectedReturn)
//   3. For each return_i, square the difference from the mean
//   4. Average those squares  →  variance
//   5. Take square root       →  standard deviation
//
// Example: returns = [-0.06, +0.02, +0.09]
//   mean     = 0.0167
//   diffs²   = (0.0767)², (0.0033)², (0.0733)²
//   variance = average of diffs² = 0.00497
//   std dev  = √0.00497 ≈ 0.0705
// ------------------------------------------------------------------
double Analyzer::computeVolatility() const
{
    const auto& fp = sim.getFinalPrices();

    // Step 1+2: compute all returns and their mean
    std::vector<double> returns;
    returns.reserve(fp.size());
    double sumR = 0.0;
    for (double price : fp) {
        double r = (price - initPrice) / initPrice;
        returns.push_back(r);
        sumR += r;
    }
    double meanR = sumR / returns.size();

    // Step 3+4: variance
    double sumSq = 0.0;
    for (double r : returns)
        sumSq += (r - meanR) * (r - meanR);

    double variance = sumSq / returns.size();

    // Step 5: std dev
    return std::sqrt(variance);
}

// ------------------------------------------------------------------
// computeLossProbability()
// ------------------------------------------------------------------
// FORMULA:
//   lossProbability = (number of sims where finalPrice < initPrice)
//                     / totalSims
//
// Interpretation:
//   0.31 → 31% chance of losing money over the simulation period
//   0.10 → only 10% chance of loss (safer stock)
//   0.60 → 60% chance of loss (very risky)
// ------------------------------------------------------------------
double Analyzer::computeLossProbability() const
{
    const auto& fp = sim.getFinalPrices();
    int lossCount = 0;

    for (double price : fp)
        if (price < initPrice) lossCount++;

    return static_cast<double>(lossCount) / fp.size();
}

// ------------------------------------------------------------------
// computeMaxDrawdown()
// ------------------------------------------------------------------
// FORMULA:
//   For EACH simulation path, find the worst peak-to-trough drop.
//   MaxDrawdown = the worst such drop across ALL paths.
//
// What is a drawdown?
//   Imagine the price goes: $150 → $160 → $170 → $140 → $155
//   Peak = $170.  Trough after that peak = $140.
//   Drawdown = ($140 - $170) / $170 = -0.176  (-17.6%)
//
// Why does this matter?
//   It tells an investor: "In the worst scenario, how much of
//   your money could temporarily disappear before recovering?"
//   A -50% drawdown is psychologically brutal even if you end
//   up profitable — most real investors would panic-sell.
//
// We scan every path and track the running peak, then compute
// the drop from that peak at each day.
// ------------------------------------------------------------------
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

    return worstDrawdown;   // e.g. -0.18
}

// ------------------------------------------------------------------
// computeWorstCase()
// ------------------------------------------------------------------
// The single lowest return seen across all simulations.
// ------------------------------------------------------------------
double Analyzer::computeWorstCase() const
{
    const auto& fp = sim.getFinalPrices();
    double worstPrice = *std::min_element(fp.begin(), fp.end());
    return (worstPrice - initPrice) / initPrice;
}

// ------------------------------------------------------------------
// computeBestCase()
// ------------------------------------------------------------------
double Analyzer::computeBestCase() const
{
    const auto& fp = sim.getFinalPrices();
    double bestPrice = *std::max_element(fp.begin(), fp.end());
    return (bestPrice - initPrice) / initPrice;
}

// ------------------------------------------------------------------
// computeAvgFinalPrice()
// ------------------------------------------------------------------
double Analyzer::computeAvgFinalPrice() const
{
    const auto& fp = sim.getFinalPrices();
    double sum = std::accumulate(fp.begin(), fp.end(), 0.0);
    return sum / fp.size();
}

// ==================================================================
//  PUBLIC INTERFACE
// ==================================================================

// ------------------------------------------------------------------
// compute() — run all helpers and cache results in `metrics`
// ------------------------------------------------------------------
// We only do the heavy work ONCE. After that, getters just read
// the cached struct.  This pattern is called "lazy evaluation"
// (though here we call it explicitly — Step 12 will call it once
// before sending data to the ML API).
// ------------------------------------------------------------------
void Analyzer::compute()
{
    if (sim.getFinalPrices().empty()) {
        std::cerr << "[Analyzer] No simulation data. "
                  << "Run Simulation::runMonteCarlo() first.\n";
        return;
    }

    metrics.expectedReturn   = computeExpectedReturn();
    metrics.volatility       = computeVolatility();
    metrics.lossProbability  = computeLossProbability();
    metrics.maxDrawdown      = computeMaxDrawdown();
    metrics.worstCase        = computeWorstCase();
    metrics.bestCase         = computeBestCase();
    metrics.avgFinalPrice    = computeAvgFinalPrice();

    computed = true;
}

// ------------------------------------------------------------------
// Getters
// ------------------------------------------------------------------
double Analyzer::getExpectedReturn()  const { return metrics.expectedReturn;  }
double Analyzer::getVolatility()      const { return metrics.volatility;      }
double Analyzer::getLossProbability() const { return metrics.lossProbability; }
double Analyzer::getMaxDrawdown()     const { return metrics.maxDrawdown;     }
double Analyzer::getWorstCase()       const { return metrics.worstCase;       }
double Analyzer::getBestCase()        const { return metrics.bestCase;        }
double Analyzer::getAvgFinalPrice()   const { return metrics.avgFinalPrice;   }

RiskMetrics Analyzer::getMetrics() const { return metrics; }

// ------------------------------------------------------------------
// printReport() — the formatted risk report
// ------------------------------------------------------------------
void Analyzer::printReport() const
{
    if (!computed) {
        std::cout << "[Analyzer] Call compute() before printReport().\n";
        return;
    }

    // ── Helper lambda: convert decimal return to % string ─────
    auto pct = [](double v) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << (v * 100.0) << "%";
        return oss.str();
    };

    // ── Risk level label based on volatility ──────────────────
    std::string riskLabel;
    if      (metrics.volatility < 0.08) riskLabel = "LOW";
    else if (metrics.volatility < 0.15) riskLabel = "MEDIUM";
    else                                 riskLabel = "HIGH";

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║           RISK ANALYSIS REPORT               ║\n";
    std::cout << "╠══════════════════════════════════════════════╣\n";
    std::cout << "║  Simulations  : "
              << std::setw(6) << sim.getNumSimulations()
              << "  (" << sim.getNumDays() << " days each)       ║\n";
    std::cout << "╠══════════════════════════════════════════════╣\n";
    std::cout << "║  RETURNS                                      ║\n";
    std::cout << "║    Expected Return   : "
              << std::setw(10) << std::right
              << pct(metrics.expectedReturn) << "                ║\n";
    std::cout << "║    Worst Case Return : "
              << std::setw(10) << std::right
              << pct(metrics.worstCase) << "                ║\n";
    std::cout << "║    Best Case Return  : "
              << std::setw(10) << std::right
              << pct(metrics.bestCase) << "                ║\n";
    std::cout << "╠══════════════════════════════════════════════╣\n";
    std::cout << "║  RISK METRICS                                 ║\n";
    std::cout << "║    Volatility        : "
              << std::setw(10) << std::right
              << std::setprecision(4) << metrics.volatility
              << "                ║\n";
    std::cout << "║    Loss Probability  : "
              << std::setw(10) << std::right
              << pct(metrics.lossProbability) << "                ║\n";
    std::cout << "║    Max Drawdown      : "
              << std::setw(10) << std::right
              << pct(metrics.maxDrawdown) << "                ║\n";
    std::cout << "╠══════════════════════════════════════════════╣\n";
    std::cout << "║  PRICE                                        ║\n";
    std::cout << "║    Avg Final Price   : $"
              << std::setw(9) << std::right << std::setprecision(2)
              << metrics.avgFinalPrice << "                ║\n";
    std::cout << "╠══════════════════════════════════════════════╣\n";
    std::cout << "║  Risk Level: " << std::left << std::setw(32)
              << riskLabel << "║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";

    // ── ML Features Preview ───────────────────────────────────
    // These are exactly the 4 numbers we'll send to the Python
    // API in Step 12
    std::cout << "\n--- ML Input Features (for API in Step 12) ---\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "  expected_return : " << metrics.expectedReturn  << "\n";
    std::cout << "  volatility      : " << metrics.volatility      << "\n";
    std::cout << "  loss_prob       : " << metrics.lossProbability  << "\n";
    std::cout << "  drawdown        : " << metrics.maxDrawdown      << "\n";
}
