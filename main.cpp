#include <iostream>
#include <string>
#include <vector>

#include "stock.h"
#include "simulation.h"
#include "analysis.h"
#include "portfolio.h"
#include "recommendation.h"

int main()
{
    bool running = true;

    while (running)
    {
        // 1. Display Menu Options
        std::cout << "\n=======================================\n";
        std::cout << "      MAIN MENU     \n";
        std::cout << "=======================================\n";
        std::cout << "1. Analyze a Single Stock\n";
        std::cout << "2. Build & Analyze a Portfolio\n";
        std::cout << "3. Compare Two Stocks Side-by-Side\n";
        std::cout << "4. Exit\n";
        std::cout << "=======================================\n";

        int choice;
        std::cout << "Enter choice [1-4]: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1: // 1. ANALYZE A SINGLE STOCK + STRATEGIC ADVICE
        {
            std::cout << "\n--- SINGLE STOCK ANALYSIS ---\n";

            std::string ticker;
            double price, vol;
            int sims, days;

            std::cout << "Enter stock ticker (e.g. AAPL): ";
            std::cin >> ticker;
            std::cout << "Enter current stock price: $";
            std::cin >> price;
            std::cout << "Enter daily volatility % (e.g. 2 for 2%): ";
            std::cin >> vol;

            vol = vol / 100.0; // convert percentage to decimal value

            std::cout << "Enter number of simulations (e.g. 1000): ";
            std::cin >> sims;
            std::cout << "Enter simulation horizon in days (e.g. 30): ";
            std::cin >> days;

            // creating objects
            Stock stock(ticker, price, vol);
            Simulation sim(stock, sims, days);
            sim.runMonteCarlo();

            Analyzer analyzer(sim);
            analyzer.compute();
            RiskMetrics m = analyzer.getMetrics();

            // Print the raw mathematical results
            std::cout << "\n--- RAW RISK RESULTS FOR " << ticker << " ---\n";
            std::cout << "Expected Return : " << m.expectedReturn << "\n";
            std::cout << "Volatility      : " << m.volatility << "\n";
            std::cout << "Loss Probability: " << m.lossProbability << "\n";
            std::cout << "Worst Case      : " << m.worstCase << "\n";
            std::cout << "Best Case       : " << m.bestCase << "\n";
            std::cout << "--------------------------------------\n";

            Recommendation recEngine;
            AIResult advice = recEngine.analyze(m);
            recEngine.printFullReport(m, advice);
            break;
        }

        case 2: // 2. BUILD & ANALYZE A PORTFOLIO
        {
            std::cout << "\n--- PORTFOLIO ANALYSIS ---\n";

            std::string portName;
            int numStocks, sims, days;

            std::cout << "Enter portfolio name: ";
            std::cin >> portName;
            std::cout << "How many stocks inside the portfolio?: ";
            std::cin >> numStocks;
            std::cout << "Enter number of simulations: ";
            std::cin >> sims;
            std::cout << "Enter simulation horizon in days: ";
            std::cin >> days;

            Portfolio port(portName, sims, days);

            for (int i = 0; i < numStocks; i++)
            {
                std::cout << "\n[Stock " << (i + 1) << " of " << numStocks << "]\n";
                std::string ticker;
                double price, vol, weight;

                std::cout << "  Ticker symbol: ";
                std::cin >> ticker;
                std::cout << "  Current price: $";
                std::cin >> price;
                std::cout << "  Daily volatility %: ";
                std::cin >> vol;
                std::cout << "  Portfolio weight % (e.g. 40 for 40%): ";
                std::cin >> weight;

                vol = vol / 100.0;
                weight = weight / 100.0;

                port.addStock(ticker, price, vol, weight);
            }

            port.simulatePortfolio();
            port.printHoldings();
            port.printReport();
            break;
        }

        case 3: // 3. COMPARE TWO STOCKS
        {
            std::cout << "\n--- SIDE-BY-SIDE COMPARISON ---\n";

            std::string tickerA, tickerB;
            double priceA, volA, priceB, volB;
            int sims, days;

            std::cout << "Enter Stock A Ticker: ";
            std::cin >> tickerA;
            std::cout << "Price A: $";
            std::cin >> priceA;
            std::cout << "Volatility A %: ";
            std::cin >> volA;

            std::cout << "\nEnter Stock B Ticker: ";
            std::cin >> tickerB;
            std::cout << "Price B: $";
            std::cin >> priceB;
            std::cout << "Volatility B %: ";
            std::cin >> volB;

            std::cout << "\nEnter simulations for comparison: ";
            std::cin >> sims;
            std::cout << "Enter trading days for comparison: ";
            std::cin >> days;

            volA = volA / 100.0;
            volB = volB / 100.0;

            // calculations for Stock A
            Stock stockA(tickerA, priceA, volA);
            Simulation simA(stockA, sims, days);
            simA.runMonteCarlo();
            Analyzer anzA(simA);
            anzA.compute();
            RiskMetrics mA = anzA.getMetrics();

            // calculations for Stock B
            Stock stockB(tickerB, priceB, volB);
            Simulation simB(stockB, sims, days);
            simB.runMonteCarlo();
            Analyzer anzB(simB);
            anzB.compute();
            RiskMetrics mB = anzB.getMetrics();

            // displaying output
            std::cout << "\n===============================================\n";
            std::cout << "METRIC            | " << tickerA << "          | " << tickerB << "\n";
            std::cout << "===============================================\n";
            std::cout << "Expected Return   | " << mA.expectedReturn << "        | " << mB.expectedReturn << "\n";
            std::cout << "Volatility        | " << mA.volatility << "        | " << mB.volatility << "\n";
            std::cout << "Loss Probability  | " << mA.lossProbability << "        | " << mB.lossProbability << "\n";
            std::cout << "Worst Case Return | " << mA.worstCase << "        | " << mB.worstCase << "\n";
            std::cout << "Best Case Return  | " << mA.bestCase << "        | " << mB.bestCase << "\n";
            std::cout << "===============================================\n";
            break;
        }

        case 4: // For Exit
            std::cout << "\nProgram Exited\n";
            running = false;
            break;

        default:
            std::cout << "Invalid Choice. Please pick a number from 1 to 4.\n";
            break;
        }
    }


    return 0;

}