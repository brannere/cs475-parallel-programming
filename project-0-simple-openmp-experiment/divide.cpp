#include <cstdlib>
#include <iostream>
#include <cstdio>

int main(int argc, char* argv[]){
  if(argc < 3){
    fprintf(stderr, "divide: not enough arguments\n");
    fprintf(stderr, "usage: numerator/denominator\n");
    return 1;
  }
  std::cout << (atof(argv[1]) / atof(argv[2])) << "\n";
  return 0;
}