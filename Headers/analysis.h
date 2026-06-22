#ifndef ANALYZER_H
#define ANALYZER_H

// ============================================================
// analyzer.h — Declaration of the Analyzer class
// STEP 4: Computes all risk metrics from simulation results
//
// METRICS PRODUCED:
//   expectedReturn   — average % gain/loss across all sims
//   volatility       — how spread out the outcomes are (std dev)
//   lossProbability  — fraction of sims that lost money
//   maxDrawdown      — worst peak-to-trough drop in any path
//   worstCase        — lowest return seen across all sims
//   bestCase         — highest return seen across all sims
//   avgFinalPrice    — mean final price across all sims
// ============================================================

#include "simulation.h"
#include <vector>

// ── A plain struct to bundle all results together ─────────────
// We'll pass this struct directly to the AI client in Step 12.
struct RiskMetrics {
    double expectedReturn;    // e.g.  0.124  means +12.4%
    double volatility;        // e.g.  0.22   (standard deviation of returns)
    double lossProbability;   // e.g.  0.31   means 31% of sims lost money
    double maxDrawdown;       // e.g. -0.18   means worst drop was -18%
    double worstCase;         // e.g. -0.21   (single worst sim return)
    double bestCase;          // e.g.  0.38   (single best sim return)
    double avgFinalPrice;     // e.g.  168.60  (mean final price in $)
};

// ── The Analyzer class ────────────────────────────────────────
class Analyzer {
private:
    const simulation& sim;     // read-only reference to simulation data
    double            initPrice;

    // Cached results (computed once, reused)
    RiskMetrics metrics;
    bool        computed;      // guard: have we run compute() yet?

    // ── Private helpers (internal calculation steps) ─────────
    double computeExpectedReturn()  const;
    double computeVolatility()      const;
    double computeLossProbability() const;
    double computeMaxDrawdown()     const;
    double computeWorstCase()       const;
    double computeBestCase()        const;
    double computeAvgFinalPrice()   const;

public:
    // Constructor — takes a completed Simulation by const reference
    explicit Analyzer(const simulation& sim);

    // Run all calculations and cache results
    void compute();

    // ── Individual getters (each calls compute() if needed) ──
    double getExpectedReturn()   const;
    double getVolatility()       const;
    double getLossProbability()  const;
    double getMaxDrawdown()      const;
    double getWorstCase()        const;
    double getBestCase()         const;
    double getAvgFinalPrice()    const;

    // Get all metrics at once (used by AIClient in Step 12)
    RiskMetrics getMetrics() const;

    // Pretty-print the full risk report
    void printReport() const;
};

#endif // ANALYZER_H
