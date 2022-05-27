#!/bin/bash



#SBATCH -J ArrayMul
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o arraymul.out
#SBATCH -e arraymul.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=brannere@oregonstate.edu

PROC1=""
PROC2=""
PROC3=""

echo "OVERWRITE making headers for csv files"

echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultsPerSecond" > dgx.first.csv
echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultsPerSecond" > dgx.second.csv
echo "NUM_ELEMENTS, LOCAL_SIZE, NUM_WORK_GROUPS, GigaMultiply-Adds Per Second" > dgx.third.csv


echo "running all three programs"
for gs in 1 512 1024 2048 4096 8192
do
  for ls in 8 16 32 64 128 256 512
  do
    g++ -w -o first first.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./first >> dgx.first.csv

    g++ -w -o second second.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./second >> dgx.second.csv

    g++ -w -o third third.cpp -DNMB=$gs -DLOCAL_SIZE=$ls /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
    ./third >> dgx.third.csv &
  done
done
