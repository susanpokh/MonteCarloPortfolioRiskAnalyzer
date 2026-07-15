// ============================================================
// simulation.cpp — Implementation of the Simulation class
// STEP 3: Monte Carlo engine
// ============================================================

#include "simulation.h"

#include <iostream>
#include <iomanip>
#include <algorithm> // std::min_element, std::max_element
#include <numeric>   // std::accumulate

Simulation::Simulation(Stock &stock, int numSimulations, int numDays)
    : stock(stock),
      numSimulations(numSimulations),
      numDays(numDays)
{

    finalPrices.reserve(numSimulations);
    allPaths.reserve(numSimulations);
}

// What happens here
//
//   Repeat numSimulations times:
//     1. Tell the stock to generate a fresh 30-day price path
//     2. Save the entire path  (allPaths)
//     3. Save just the last price  (finalPrices)
//
// After the loop, finalPrices looks like:
//   [163.10, 141.88, 172.55, 134.20, 158.44, ...]   ← 1000 values
//
// Each value is where the stock *could* end up after 30 days.
// Some are up, some are down — the spread of outcomes is the risk.
// ------------------------------------------------------------------




void Simulation::runMonteCarlo()
{
    // Clear any previous results so runMonteCarlo() can be called
    // multiple times safely
    finalPrices.clear();
    allPaths.clear();

    std::cout << "Running " << numSimulations
              << " simulations (" << numDays << " days each)";
    std::cout.flush(); // print the line without a newline yet

    for (int i = 0; i < numSimulations; i++)
    {
        // ── Progress dots every 100 runs ──────────────────────
        if (i % 100 == 0)
        {
            std::cout << ".";
            std::cout.flush();
        }

        // ── 1. Generate a complete price path ─────────────────
        // generatePricePath() calls reset() internally, so each
        // run starts fresh from initialPrice.
        std::vector<double> path = stock.generatePricePath(numDays);

        // ── 2. Store the full path ─────────────────────────────
        allPaths.push_back(path);

        // ── 3. Store only the final price ─────────────────────
        // path.back() = last element = price on day numDays
        finalPrices.push_back(path.back());
    }

    std::cout << " Done!\n";
}

void Simulation::printSummary() const
{
    if (finalPrices.empty())
    {
        std::cout << "[Simulation] No results yet. Call runMonteCarlo() first.\n";
        return;
    }

    double initPrice = stock.getInitialPrice();

    // ── Aggregate statistics across all final prices ──────────

    // 1. Average final price
    double sum = std::accumulate(finalPrices.begin(), finalPrices.end(), 0.0);
    double avgFinal = sum / finalPrices.size();

    // 2. Worst and best outcome
    double worstPrice = *std::min_element(finalPrices.begin(), finalPrices.end());
    double bestPrice = *std::max_element(finalPrices.begin(), finalPrices.end());

    // 3. Convert prices to returns (%)
    double avgReturn = (avgFinal - initPrice) / initPrice * 100.0;
    double worstReturn = (worstPrice - initPrice) / initPrice * 100.0;
    double bestReturn = (bestPrice - initPrice) / initPrice * 100.0;

    // 4. Loss probability = fraction of simulations that ended below
    //    the starting price
    int lossSims = 0;
    for (double p : finalPrices)
        if (p < initPrice)
            lossSims++;

    double lossProb = static_cast<double>(lossSims) / finalPrices.size();

    // ── Print ─────────────────────────────────────────────────
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║       MONTE CARLO SIMULATION RESULTS     ║\n";
    std::cout << "╠══════════════════════════════════════════╣\n";
    std::cout << "║  Stock           : " << std::setw(22) << std::left
              << stock.getName() << "║\n";
    std::cout << "║  Simulations     : " << std::setw(22) << std::left
              << numSimulations << "║\n";
    std::cout << "║  Days per run    : " << std::setw(22) << std::left
              << numDays << "║\n";
    std::cout << "╠══════════════════════════════════════════╣\n";

    std::cout << "║  Start Price     : $" << std::right << std::setw(8)
              << initPrice << "                ║\n";
    std::cout << "║  Avg Final Price : $" << std::right << std::setw(8)
              << avgFinal << "                ║\n";
    std::cout << "║  Worst Final     : $" << std::right << std::setw(8)
              << worstPrice << "                ║\n";
    std::cout << "║  Best Final      : $" << std::right << std::setw(8)
              << bestPrice << "                ║\n";
    std::cout << "╠══════════════════════════════════════════╣\n";
    std::cout << "║  Avg Return      : " << std::right << std::setw(7)
              << avgReturn << "%               ║\n";
    std::cout << "║  Worst Return    : " << std::right << std::setw(7)
              << worstReturn << "%               ║\n";
    std::cout << "║  Best Return     : " << std::right << std::setw(7)
              << bestReturn << "%               ║\n";
    std::cout << "║  Loss Probability: " << std::right << std::setw(7)
              << lossProb * 100.0 << "%               ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";

    std::cout << "\n--- Outcome Distribution (final prices) ---\n";

    const int BUCKETS = 10;
    const int BAR_MAX = 30;

    double bucketWidth = (bestPrice - worstPrice) / BUCKETS;
    if (bucketWidth == 0)
        bucketWidth = 1.0;

    std::vector<int> counts(BUCKETS, 0);
    for (double p : finalPrices)
    {
        int b = static_cast<int>((p - worstPrice) / bucketWidth);
        if (b >= BUCKETS)
            b = BUCKETS - 1; // clamp the max
        counts[b]++;
    }

    int maxCount = *std::max_element(counts.begin(), counts.end());

    for (int b = 0; b < BUCKETS; b++)
    {
        double lo = worstPrice + b * bucketWidth;
        double hi = lo + bucketWidth;
        int barLen = (maxCount > 0)
                         ? (counts[b] * BAR_MAX / maxCount)
                         : 0;

        std::cout << "$" << std::setw(7) << lo
                  << " - $" << std::setw(7) << hi
                  << " | ";
        for (int x = 0; x < barLen; x++)
            std::cout << '#';
        std::cout << " (" << counts[b] << ")\n";
    }
}

const std::vector<double> &Simulation::getFinalPrices() const
{
    return finalPrices;
}

const std::vector<std::vector<double>> &Simulation::getAllPaths() const
{
    return allPaths;
}

int Simulation::getNumSimulations() const { return numSimulations; }
int Simulation::getNumDays() const { return numDays; }
