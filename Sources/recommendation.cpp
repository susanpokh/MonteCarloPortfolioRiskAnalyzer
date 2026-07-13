#include "recommendation.h"
#include <iostream>

// Classify risk level based on your threshold variables
std::string Recommendation::classifyRiskLevel(const RiskMetrics &m) const {
    if (m.volatility >= VOL_HIGH || m.lossProbability >= LP_DANGER) {
        return "High Risk";
    } else if (m.volatility >= VOL_MEDIUM || m.lossProbability >= LP_CAUTION) {
        return "Medium Risk";
    } else {
        return "Low Risk";
    }
}

// Compute simple ratio-based voting confidence 
double Recommendation::computeConfidence(int buyVotes, int sellVotes, int totalVotes) const {
    if (totalVotes == 0) return 0.0;
    
    // Pick the winning side's weight relative to the total number of conditions checked
    int maxVotes = (buyVotes > sellVotes) ? buyVotes : sellVotes;
    if (maxVotes == 0) return 0.50; // Neutral hold fallback
    
    return static_cast<double>(maxVotes) / totalVotes;
}

// Build human-readable reasoning based on the thresholds
std::string Recommendation::buildReasoning(const RiskMetrics &m, const std::string &rec, const std::string &risk) const {
    std::string reason = "Asset displays a " + risk + " profile. ";
    
    if (rec == "Strong Buy" || rec == "Buy") {
        reason += "Driven by positive expected returns (" + std::to_string(m.expectedReturn * 100.0) + "%) ";
        if (m.lossProbability < LP_SAFE) {
            reason += "and a safely constrained loss probability.";
        }
    } else if (rec == "Strong Sell" || rec == "Sell") {
        reason += "Downside metrics triggered warnings due to negative expected growth or heavy drawdowns.";
    } else {
        reason += "Consolidating metrics point toward stable holding patterns.";
    }
    return reason;
}

// Core calculation module mapping metrics onto strategic choices
AIResult Recommendation::analyze(const RiskMetrics &metrics) const {
    AIResult result;
    result.riskLevel = classifyRiskLevel(metrics);

    int buyVotes = 0;
    int sellVotes = 0;
    int totalVotes = 3; // Checking 3 conditions: Return, Loss Probability, and Max Drawdown

    // 1. Vote on Expected Return
    if (metrics.expectedReturn >= ER_STRONG_BUY) buyVotes += 2;
    else if (metrics.expectedReturn >= ER_BUY) buyVotes += 1;
    else if (metrics.expectedReturn <= ER_STRONG_SELL) sellVotes += 2;
    else if (metrics.expectedReturn <= ER_SELL) sellVotes += 1;

    // 2. Vote on Loss Probability
    if (metrics.lossProbability < LP_SAFE) buyVotes += 1;
    else if (metrics.lossProbability > LP_DANGER) sellVotes += 1;

    // 3. Vote on Max Drawdown (Worst Case)
    // Note: Since worstCase is typically a negative return fraction, -0.05 is greater than -0.10 (shallow)
    if (metrics.worstCase >= DD_SHALLOW) buyVotes += 1;
    else if (metrics.worstCase <= DD_DEEP) sellVotes += 1;

    // Determine final recommendation label based on net votes
    if (buyVotes >= 3) result.recommendation = "Strong Buy";
    else if (buyVotes > sellVotes) result.recommendation = "Buy";
    else if (sellVotes >= 3) result.recommendation = "Strong Sell";
    else if (sellVotes > buyVotes) result.recommendation = "Sell";
    else result.recommendation = "Hold";

    // Compute metrics
    result.confidence = computeConfidence(buyVotes, sellVotes, buyVotes + sellVotes);
    result.reasoning = buildReasoning(metrics, result.recommendation, result.riskLevel);

    return result;
}

void Recommendation::printResult(const AIResult &result) const {
    std::cout << "\n=============================================\n";
    std::cout << "        RECOMMENDATION ENGINE OUTPUT        \n";
    std::cout << "=============================================\n";
    std::cout << "Action Signal: " << result.recommendation << "\n";
    std::cout << "Risk Profile : " << result.riskLevel << "\n";
    std::cout << "Confidence   : " << (result.confidence * 100.0) << "%\n";
    std::cout << "=============================================\n";
}

void Recommendation::printFullReport(const RiskMetrics &m, const AIResult &result) const {
    printResult(result);
    std::cout << "Reasoning    : " << result.reasoning << "\n";
    std::cout << "=============================================\n";
}