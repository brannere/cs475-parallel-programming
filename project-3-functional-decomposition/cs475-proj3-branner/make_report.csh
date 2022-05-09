#!/bin/csh
# g++ -fopenmp main.cpp  -lm -O3 -o zoo
/opt/homebrew/opt/llvm/bin/clang -lc++ -fopenmp -L /opt/homebrew/lib main.cpp -lm -O3 -o zoo
./zoo
