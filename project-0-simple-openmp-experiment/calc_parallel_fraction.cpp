#include <cstdlib>
#include <iostream>
#include <cstdio>

int main(int argc, char* argv[]){
  
  if(argc < 2){
    fprintf(stderr, "calc_parallel_fraction: not enough arguments\n");
    return 1;
  }
  float s = atof(argv[1]);
  std::cout << (4./3.)*( 1. - (1./s) ) << "\n";
  return 0;
}