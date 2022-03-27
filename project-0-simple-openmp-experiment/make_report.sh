#!/bin/bash

array_size=16384
num_trials=10000

make clean
make

one_thread_performance=`./run 1 $array_size $num_trials`
four_thread_performance=`./run 4 $array_size $num_trials`

speedup=`./divide $four_thread_performance $one_thread_performance`
parallel_fraction=`./calc_parallel_fraction $speedup`
echo
echo "--RESULTS--"
echo "One thread performance: $one_thread_performance"
echo "Four thread performance: $four_thread_performance"
echo
echo "speedup: $speedup"
echo "parallel fraction: $parallel_fraction"
