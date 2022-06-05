#!/bin/bash

for numCpus in 2 4 8 16 32 64
do
    sbatch ./submit.bash numCpus
done
