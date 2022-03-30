#!/bin/csh
echo "threads,trials,probability,megatrials/sec" 
foreach t ( 1 2 4 8 12 16 20 24 32 )
  foreach n ( 1 10 100 1000 10000 100000 500000 1000000 )
        /opt/homebrew/opt/llvm/bin/clang++ -fopenmp -L /opt/homebrew/lib main.cpp -DNUMT=$t -DNUMTRIALS=$n -o montecarlo
    ./montecarlo
  end
end
