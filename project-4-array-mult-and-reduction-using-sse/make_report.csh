#!/bin/csh


foreach arrSize  (1000 2000 4000 8000 16000 32000 64000 180000 256000 512000 1024000)
       g++ -g -lm -fopenmp main.cpp -DARRAYSIZE=$arrSize -o ./run
       ./run
end
