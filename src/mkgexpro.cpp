#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <argp.h>

#include "gexpro.hpp"
#include "geoParser.hpp"


int main (int argc, char* argv[]) {

  std::cout << "Number of arguments passed: " << argc << std::endl;
  std::cout << "Arguments text: " << *argv << std::endl;

  GeoParser parser;

  const std::string testpro_fname = "../data/geo/GDS1517_full.soft";
  Gexpro gds1517 = parser.parseFile(testpro_fname);

  std::cout << "Your profile is named " << gds1517.getName() << std::endl;

  // Write data matrix to text file
  gds1517.dumpMatrix();
  
  return 0;
}
