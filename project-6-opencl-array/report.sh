#!/bin/bash

PROC1=""
PROC2=""
PROC3=""

echo "OVERWRITE making headers for csv files"

echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultsPerSecond" > first.csv
echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultsPerSecond" > second.csv
echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultiply-Adds Per Second" > third.csv


echo "running all three programs"
for gs in 1 512 1024 2048 4096 8192
do
  for ls in 8 16 32 64 128 256 512
  do
    g++ -w -o first first.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./first >> first.csv
    PROC1=$!

    g++ -w -o second second.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./second >> second.csv
    PROC2=$!

    g++ -w -o third third.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./third >> third.csv
    PROC3=$!
    wait $PROC1 $PROC2 $PROC3
  done
done
