#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "gexpro.hpp"

int main (int argc, char *argv[]) {
  std::string gexproname = "test";

  SpMat<double> &nameStr = sprandu(1000, 2000, 0.01);
  Col<Gene>
  Gexpro *g = new Gexpro(gexproname);

  std::cout << "Your Gexpro instance is named " << g->getName() << std::endl;
  
  return 0;
}
