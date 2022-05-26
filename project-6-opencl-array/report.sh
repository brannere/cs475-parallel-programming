#!/bin/bash
echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultsPerSecond"

for gs in 1 512 1024 2048 4096 8192
do
  for ls in 8 16 32 64 128 256 512
  do
    g++ -w -o first first.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./first >> first.csv
    
    g++ -w -o second second.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./second >> second.csv
    
    # g++ -w -o third third.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    # ./third
    
  done
done