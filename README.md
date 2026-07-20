# Monte Carlo Portfolio Risk Analyzer

A command-line tool written in C++ that estimates the risk of a stock or a small
portfolio by running Monte Carlo simulations, then turns the results into a plain
buy / hold / sell suggestion.

The idea is simple: instead of guessing how risky a position is, you simulate
thousands of possible futures for it and look at the spread of outcomes. I wrote
it mostly to get comfortable with object-oriented C++ and to have a small,
self-contained project that actually does something.

## What it does

From the main menu you can:

- **Analyze a single stock.** Enter a ticker, price, daily volatility and a time
  horizon, and it reports the expected return, volatility, loss probability,
  best/worst case, and a recommendation with a short reason.
- **Build and analyze a portfolio.** Add several stocks with weights (they should
  add up to 100%). It simulates each one and blends the returns into a single
  combined risk profile.
- **Compare two stocks side by side.** Runs both through the same simulation and
  lines up their metrics so you can see which one is riskier.

## How the simulation works

Each simulation builds a day-by-day price path. Every day the price is nudged up
or down by a random amount scaled to the stock's volatility, plus a small upward
drift. Run that for, say, 30 days and you get one possible ending price. Repeat it
a thousand times and you have a whole distribution of where the stock might land.

The risk numbers all come out of that distribution:

- **Expected return** — average gain/loss across every run
- **Volatility** — standard deviation of those returns
- **Loss probability** — share of runs that finished below the starting price
- **Max drawdown** — worst peak-to-trough drop on any single path
- **Best / worst case** — the highest and lowest returns seen

One honest caveat: the price model here is a simplified bounded random walk, not a
proper Geometric Brownian Motion, and it isn't fitted to real market data. It's
enough to demonstrate the idea and produce sensible-looking distributions, but
don't read the output as actual investment advice. 

## The recommendation part

Despite the struct being named `AIResult`, there's no machine learning here. It's
a small rule-based voting system. Three signals — expected return, loss
probability, and drawdown — each cast a vote against fixed thresholds, with strong
moves counting double. The votes get tallied into one of five actions (Strong Buy,
Buy, Hold, Sell, Strong Sell), and the confidence score is just the winning side's
share of the votes. It's deterministic and easy to follow on purpose, so you can
always trace why it said what it said.

## Building and running

You need a C++ compiler (g++ or clang) with C++11 support or newer.

Compile all the sources into one binary and run it:

```bash
g++ -std=c++11 -I./Headers \
    Sources/portfolio.cpp \
    Sources/stock.cpp \
    Sources/simulation.cpp \
    Sources/analysis.cpp \
    Sources/recommendation.cpp \
    main.cpp \
    -o risk_analyzer
./risk_analyzer
```

There's also a `build/build.sh` that runs the same compile command if you'd rather
not type it out:

```bash
bash build/build.sh
```

After that, just follow the menu.

## Sample run

```
=======================================
              MAIN MENU
=======================================
1. Analyze a Single Stock
2. Build & Analyze a Portfolio
3. Compare Two Stocks Side-by-Side
4. Exit
=======================================
Enter choice [1-4]: 1

--- SINGLE STOCK ANALYSIS ---
Enter stock ticker (e.g. AAPL): AAPL
Enter current stock price: $180
Enter daily volatility % (e.g. 2 for 2%): 2
Enter number of simulations (e.g. 1000): 1000
Enter simulation horizon in days (e.g. 30): 30
Running 1000 simulations (30 days each).......... Done!

--- RAW RISK RESULTS FOR AAPL ---
Expected Return : ...
Volatility      : ...
Loss Probability: ...
Worst Case      : ...
Best Case       : ...
```

The numbers shift a little from run to run since the paths are random.

## Project layout

```
Headers/            class declarations
  stock.h           one asset: price, volatility, and its path generator
  simulation.h      the Monte Carlo engine
  analysis.h        risk-metric math + the RiskMetrics struct
  portfolio.h       several weighted stocks combined
  recommendation.h  the rule-based buy/hold/sell engine
Sources/            matching implementations for each header
main.cpp            the menu and program flow
build/build.sh      one-shot compile script
```

Each class has one job. `Stock` generates paths, `Simulation` runs them,
`Analyzer` crunches the numbers, `Recommendation` makes the call, and `Portfolio`
stitches several stocks together. The header files have comments walking through
the formulas if you want the details.

## Limitations

Being upfront about what this is and isn't:

- The price model is a bounded random walk, not real GBM, and it's driven by a
  volatility figure you type in rather than one estimated from price history. Treat
  the results as illustrative.
- Stocks in a portfolio are simulated independently, so correlation between assets
  is ignored. A real portfolio's diversification effect isn't captured.
- No Value-at-Risk or Sharpe ratio yet.
- Input handling is basic; odd values (negative prices, weights that don't sum to
  100%) mostly get a warning rather than a hard stop.

Those are the first things I'd tackle if I keep building on it.

## About

This began as a coursework / thesis project, so it's written to be read and
explained as much as run. If something isn't clear, start with the comments in the
`Headers` files.
