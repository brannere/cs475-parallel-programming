#!/bin/csh

#g++ -fopenmp main.cpp -DNUMT=$t -DNUMTRIALS=$n -lm -O3 -o montecarlo
#/opt/homebrew/opt/llvm/bin/clang -lc++ -fopenmp -L /opt/homebrew/lib
# echo "threads,trials,probability,megatrials/sec" 
# foreach t ( 1 2 4 8 12 16 )
  # /opt/homebrew/opt/llvm/bin/clang -lc++ -fopenmp -L /opt/homebrew/lib main.cpp -DNUMT=$t -DNUMTRIALS=$n -lm -O3 -o montecarlo
  # g++ -fopenmp main.cpp -lm -O3 -o integration
  # ./integration
# end

echo "threads,nodes,volume,MegaHeights computer per second" 
foreach numThreads (1 2 4 8 12 16)
  foreach numNodes (1000 2000 3000 4000 5000 6000 7000 8000 9000 10000)
    # g++ -fopenmp main.cpp -DNUMT=$numThreads -DNUMNODES=$numNodes -lm -O3 -o integration
    /opt/homebrew/opt/llvm/bin/clang -lc++ -fopenmp -L /opt/homebrew/lib main.cpp -DNUMT=$numThreads -DNUMNODES=$numNodes -lm -O3 -o integration
    ./integration
  end
end
