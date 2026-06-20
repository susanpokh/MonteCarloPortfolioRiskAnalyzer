That flowchart_1.pdf is the simple overview of what our project actually does.
flowchart_2.pdf is actually the one that describes our entire project at once (We'll show this ma'am after successfully completing the project)

I have thoroughly divided the project into 5 main modules, each of you can take one specific module and design the module

Person 1 will handle stock.h / stock.cpp files
Person 2 will handle simulation.h / simulation.cpp
Manav will handle analyzer.h / analyzer.cpp
Person 4 will handle portfolio.h / portfolio.cpp
Person 5 will handle recommendation.h / recommendation.cpp

We all will design the header files (.h) first and then everyone will go into their own respective .cpp files

On our Upcoming Meeting with Rajani Ma'am we will show the flowchart for the project and each of us will explain our chosen module
If feasible we will show the design of the header files (.h) of our module, and explain it to her

Aaditya:
    Responsibility: Build the raw asset data model.
    Key Tasks: Write the constructor, getters, and the simulate() function using random-walk or geometric Brownian motion logic. Write generatePricePath() to return a vector of daily values.
    Note: Everyone else's code depends on Person 1's Stock object working properly

Phunyal:
    Responsibility: Handle heavy parallel processing loops
    Key Tasks: Take a reference to Person 1's stock ( Stock& ), pre-allocate vectors using reserve(), and run the macro for loop to scale the simulations up to 10,000 runs, storing results in matrix formats(allPaths and finalPrices)

Manav:
    Responsibility: Process raw numbers into math metrics
    Key Tasks: Read Person 2's completed Simulation matrices and write the math functions for Expected Return, Volatility (Standard Deviation), Maximum Drawdown, and Loss Probability, bundling them into the RiskMetrics Struct.

Susan: 
    Responsibility: Manage Multi-asset blending.
    Key Tasks: Create the StockEntry composition array, handle user-allocated weights, simulate all individual stocks in paralle, and execute the linear combination math to find the combined portfolio equity curve.

Aakankshya:
    Responsibility: Write the rule-based decision algorithm.
    Key Tasks: Write the logic that takes a RiskMetrics struct and gives a final recommendation vector.


KEY TASKS that I mentioned above for each Person can be written in the LOG SHEET for each weeks

Designing header files first is really important mind that guys. And size we are late, and it's been like 1 month that we haven't met our supervisor, we should show her that we did something tangible, so if possible we will show the header files to her in upcoming meeting on Tuesday.


AFter we all have agreed and designed on our headers, we will begin writing the code inside .cpp file

The code of headers and .cpp files is already given to you as a reference, you can take help from it, understand the concepts and write the code on your own
