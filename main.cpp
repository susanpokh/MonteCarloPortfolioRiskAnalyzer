#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue()
{
    std::cout << "\n  Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int getIntInput(const std::string& prompt, int minVal, int maxVal)
{
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val && val >= minVal && val <= maxVal) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Please enter a number between " << minVal << " and " << maxVal << ".\n";
    }
}

void printBanner()
{
    std::cout << "\n";
    std::cout << "  ╔═══════════════════════════════════════════════════╗\n";
    std::cout << "  ║                                                   ║\n";
    std::cout << "  ║       Monte Carlo Portfolio Risk Analyzer         ║\n";
    std::cout << "  ║                                                   ║\n";
    std::cout << "  ╚═══════════════════════════════════════════════════╝\n";
    std::cout << "\n";
}

void printAbout()
{
    std::cout << "\n  ── ABOUT THIS PROGRAM ─────────────────────\n\n";
    std::cout << "  Author: Aaditya Dahal, Prashish Phunyal, Susan Pokharel, Aakankshya Shrestha, Manav Sharma\n\n";
    std::cout << "  Description: A financial analysis tool that simulates potential stock and portfolio price paths\n";
    std::cout << "               using Monte Carlo logic to calculate key risk metrics, drawdown probabilities,\n";
    std::cout << "               and AI-driven asset recommendations.\n\n";
}

void printMainMenu()
{
    std::cout << "  ┌─────────────────────────────────────┐\n";
    std::cout << "  │           MAIN MENU                 │\n";
    std::cout << "  ├─────────────────────────────────────┤\n";
    std::cout << "  │  1.  Analyze a Single Stock         │\n";
    std::cout << "  │  2.  Build & Analyze a Portfolio    │\n";
    std::cout << "  │  3.  Quick Demo (Preset Examples)   │\n";
    std::cout << "  │  4.  Compare Two Stocks             │\n";
    std::cout << "  │  5.  Exit                           │\n";
    std::cout << "  └─────────────────────────────────────┘\n";
    std::cout << "\n";
}

void chooseSimulations()
{
    std::cout << "\n  How many simulations?\n";
    std::cout << "    1.  1,000   (fast,   ~1 sec)\n";
    std::cout << "    2.  5,000   (normal, ~2 sec)\n";
    std::cout << "    3.  10,000  (slow,   ~5 sec)\n";
    getIntInput("  Choice [1-3]: ", 1, 3);
}

void chooseDays()
{
    std::cout << "\n  Simulation horizon (trading days)?\n";
    std::cout << "    1.  30 days  (~1 month)\n";
    std::cout << "    2.  90 days  (~1 quarter)\n";
    std::cout << "    3.  252 days (~1 year)\n";
    getIntInput("  Choice [1-3]: ", 1, 3);
}

void printProgress(const std::string& label)
{
    std::cout << "\n  " << label << "\n  [";
    for (int i = 0; i < 30; i++) {
        std::cout << "=" ;
    }
    std::cout << "] Done!\n\n";
}


void runSingleStockAnalysis()
{
    clearScreen();
    std::cout << "\n  ── SINGLE STOCK ANALYSIS ──────────────────\n\n";
    chooseSimulations();
    chooseDays();

    printProgress("Calculating risk metrics...");

    // ── MOCK HISTOGRAM OUTPUT ──────────────────────────────
    std::cout << "\n  ── HISTOGRAM ──────────────────────────────\n";
    std::cout << "  $  132.40 → $  141.10  |██████ (241)\n";
    std::cout << "  $  141.10 → $  149.80  |████████████████████ (784)\n";
    std::cout << "  $  149.80 → $  158.50  |████████████████████████████ (1102)\n";
    std::cout << "  $  158.50 → $  167.20  |█████████████████ (654)\n";
    std::cout << "  $  167.20 → $  175.90  |████ (182)\n";

    pressEnterToContinue();
}

void runComparison()
{
    clearScreen();
    std::cout << "\n  ── COMPARE TWO STOCKS ─────────────────────\n\n";
    chooseSimulations();
    chooseDays();

    printProgress("Comparing...");

    // ── SIDE-BY-SIDE MOCK REPORT ──────────────────────────────
    std::cout << "\n  ╔══════════════════════════════════════════════════════╗\n";
    std::cout << "  ║              STOCK COMPARISON REPORT                ║\n";
    std::cout << "  ╠══════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Metric                AAPL          TSLA          ║\n";
    std::cout << "  ╠══════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Expected Return       12.45%        24.10%        ║\n";
    std::cout << "  ║  Worst Case            -8.20%        -32.50%       ║\n";
    std::cout << "  ║  Best Case             32.11%        85.40%        ║\n";
    std::cout << "  ║  Volatility            0.0185        0.0412        ║\n";
    std::cout << "  ║  Loss Probability      15.20%        38.60%        ║\n";
    std::cout << "  ║  Max Drawdown          11.40%        28.95%        ║\n";
    std::cout << "  ╠══════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Risk Level            MODERATE      HIGH          ║\n";
    std::cout << "  ║  Recommendation        BUY           HOLD          ║\n";
    std::cout << "  ║  Confidence            8.45          6.20          ║\n";
    std::cout << "  ╚══════════════════════════════════════════════════════╝\n";

    pressEnterToContinue();
}

void runDemoSummary()
{
    clearScreen();
    std::cout << "\n  ── DEMO SUMMARY ───────────────────────────\n\n";
    std::cout << "  "
              << std::left  << std::setw(34) << "Portfolio"
              << std::right << std::setw(10) << "Exp.Ret"
              << std::setw(8)  << "Vol"
              << std::setw(8)  << "Risk"
              << std::setw(8)  << "Rec\n";
    std::cout << "  " << std::string(68, '-') << "\n";

    std::cout << "  " << std::left << std::setw(34) << "Conservative Income Portfolio" << std::right << "4.20%    0.0088   LOW     BUY\n";
    std::cout << "  " << std::left << std::setw(34) << "Balanced Tech Portfolio"       << std::right << "14.55%    0.0195   MOD     BUY\n";
    std::cout << "  " << std::left << std::setw(34) << "Aggressive Growth Portfolio"   << std::right << "28.10%    0.0520   HIGH    HOLD\n";

    pressEnterToContinue();
}

int main()
{
    clearScreen();
    printBanner();
    printAbout();
    pressEnterToContinue();

    bool running = true;
    while (running)
    {
        clearScreen();
        printBanner();
        printMainMenu();

        int choice = getIntInput("  Enter choice [1-5]: ", 1, 5);

        switch (choice)
        {
            case 1:
                runSingleStockAnalysis();
                break;
            case 2:
                runSingleStockAnalysis(); // Portfolio routes here to demo configuration elements
                break;
            case 3:
                runDemoSummary();
                break;
            case 4:
                runComparison();
                break;
            case 5:
                clearScreen();
                std::cout << "\n";
                std::cout << "  ╔═══════════════════════════════════╗\n";
                std::cout << "  ║   Thank you for using the         ║\n";
                std::cout << "  ║   Monte Carlo Risk Analyzer!      ║\n";
                std::cout << "  ║                                   ║\n";
                std::cout << "  ║   Remember: Past simulations      ║\n";
                std::cout << "  ║   do not guarantee future gains.  ║\n";
                std::cout << "  ╚═══════════════════════════════════╝\n\n";
                running = false;
                break;
        }
    }

    return 0;
}