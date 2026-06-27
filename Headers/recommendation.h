#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

// ============================================================
// recommendation.h — Declaration of the Recommendation class
// STEP 6: Rule-based recommender — pure C++, no Python needed
//
// WHAT THIS CLASS DOES:
//   Takes a RiskMetrics struct (from Analyzer or Portfolio)
//   and produces:
//     • recommendation  — BUY / HOLD / SELL
//     • riskLevel       — LOW / MEDIUM / HIGH
//     • confidence      — 0.0 to 1.0
//     • reasoning       — human-readable explanation
//
// HOW IT DECIDES:
//   It runs a weighted scoring system across 4 metrics.
//   Each metric casts votes toward BUY or SELL.
//   The margin between the two scores determines confidence.
//   This mirrors exactly how a Decision Tree would behave.
// ============================================================

#include "analysis.h"   // for RiskMetrics struct
#include <string>

// ── Result bundle returned by the engine ─────────────────────
struct AIResult {
    std::string recommendation;  // "BUY", "HOLD", or "SELL"
    std::string riskLevel;       // "LOW", "MEDIUM", or "HIGH"
    double      confidence;      // 0.0 – 1.0
    std::string reasoning;       // plain-English explanation
};

// ── The Recommendation class ────────────────────────────────────────
class Recommendation {
private:
    // ── Scoring thresholds (tuned to match ML model rules) ───
    // Expected return thresholds
    static constexpr double ER_STRONG_BUY  =  0.08;  // > 8%  → strong buy signal
    static constexpr double ER_BUY         =  0.03;  // > 3%  → buy signal
    static constexpr double ER_SELL        = -0.01;  // < -1% → sell signal
    static constexpr double ER_STRONG_SELL = -0.05;  // < -5% → strong sell signal

    // Volatility thresholds
    static constexpr double VOL_LOW        =  0.05;  // < 5%  → low risk
    static constexpr double VOL_MEDIUM     =  0.10;  // < 10% → medium risk
    static constexpr double VOL_HIGH       =  0.18;  // > 18% → high risk

    // Loss probability thresholds
    static constexpr double LP_SAFE        =  0.35;  // < 35% → safe
    static constexpr double LP_CAUTION     =  0.50;  // > 50% → caution
    static constexpr double LP_DANGER      =  0.62;  // > 62% → danger

    // Drawdown thresholds
    static constexpr double DD_SHALLOW     = -0.10;  // > -10% → shallow (good)
    static constexpr double DD_MODERATE    = -0.20;  // > -20% → moderate
    static constexpr double DD_DEEP        = -0.30;  // < -30% → deep (bad)

    // ── Private helpers ───────────────────────────────────────
    std::string classifyRiskLevel(const RiskMetrics& m) const;
    double      computeConfidence(int buyVotes,
                                  int sellVotes,
                                  int totalVotes) const;
    std::string buildReasoning(const RiskMetrics& m,
                               const std::string& rec,
                               const std::string& risk) const;

public:
    Recommendation() = default;   // no setup needed — stateless class

    // ── Main entry point ──────────────────────────────────────
    // Pass any RiskMetrics (from Analyzer or Portfolio)
    // and get back a complete AIResult
    AIResult analyze(const RiskMetrics& metrics) const;

    // ── Display helpers ───────────────────────────────────────
    void printResult(const AIResult& result)           const;
    void printFullReport(const RiskMetrics& m,
                         const AIResult& result)       const;
};

#endif // RECOMMENDATION_H
