#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H


#include "analysis.h"   // for RiskMetrics struct
#include <string>


struct AIResult {
    std::string recommendation;  // Buy, hold or sell
    std::string riskLevel;       // Low, medium or high
    double      confidence;      // 0 to 1
    std::string reasoning;       // explanation
};


class Recommendation {
private:

    static constexpr double ER_STRONG_BUY  =  0.08;  // > 8%  -> strong buy signal
    static constexpr double ER_BUY         =  0.03;  // > 3%  -> buy signal
    static constexpr double ER_SELL        = -0.01;  // < -1% -> sell signal
    static constexpr double ER_STRONG_SELL = -0.05;  // < -5% -> strong sell signal

    // Volatility thresholds
    static constexpr double VOL_LOW        =  0.05;  // < 5%  -> low risk
    static constexpr double VOL_MEDIUM     =  0.10;  // < 10% -> medium risk
    static constexpr double VOL_HIGH       =  0.18;  // > 18% -> high risk

    // Loss probability thresholds
    static constexpr double LP_SAFE        =  0.35;  // < 35% -> safe
    static constexpr double LP_CAUTION     =  0.50;  // > 50% -> caution
    static constexpr double LP_DANGER      =  0.62;  // > 62% -> danger

    // Drawdown thresholds
    static constexpr double DD_SHALLOW     = -0.10;  // > -10% -> shallow (good)
    static constexpr double DD_MODERATE    = -0.20;  // > -20% -> moderate
    static constexpr double DD_DEEP        = -0.30;  // < -30% -> deep (bad)


    std::string classifyRiskLevel(const RiskMetrics& m) const;
    double      computeConfidence(int buyVotes,
                                  int sellVotes,
                                  int totalVotes) const;
    std::string buildReasoning(const RiskMetrics& m,
                               const std::string& rec,
                               const std::string& risk) const;

public:
    Recommendation() = default; 

    AIResult analyze(const RiskMetrics& metrics) const;


    void printResult(const AIResult& result)           const;
    void printFullReport(const RiskMetrics& m,
                         const AIResult& result)       const;
};

#endif 
