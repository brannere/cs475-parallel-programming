#!/bin/bash

g++ -o run first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
