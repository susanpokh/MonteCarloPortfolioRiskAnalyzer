#!/bin/bash

# Compile all files together
g++ -std=c++11 -I./Headers Sources/portfolio.cpp Sources/stock.cpp main.cpp -o demo

# If compilation succeeded, run it immediately
if [ $? -eq 0 ]; then
    ./demo
fi