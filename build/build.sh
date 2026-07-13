
g++ -std=c++11 -I./Headers \
    ./Sources/portfolio.cpp \
    ./Sources/stock.cpp \
    ./Sources/simulation.cpp \
    ./Sources/analysis.cpp \
    ./main.cpp \
    -o risk_analyzer_demo

    
if [ $? -eq 0 ]; then
    ./build/risk_analyzer_demo
fi