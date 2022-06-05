#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o autocorr.out
#SBATCH -e autocorr.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=brannere@oregonstate.edu

module load openmpi

for numCpus in 1 2 4 8
do
    mpic++ autocorr.cpp -o autocorr -lm
    mpiexec -mca btl self,tcp -np $numCpus ./autocorr
done
