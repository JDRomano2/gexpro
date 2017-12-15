#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "gexpro.hpp"
//#include "parser.hpp"
#include "geoParser.hpp"

int main (int argc, char *argv[]) {

  GeoParser parser;


  const std::string testpro_fname = "../data/geo/GDS1517_full.soft";
  Gexpro gds1517 = parser.parseFile(testpro_fname);

  std::cout << "Your profile is named " << gds1517.getName() << std::endl;

  std::cout << "First 5 rows and 5 columns of the data matrix:" << std::endl;
  gds1517.printDataHeader();
  
  return 0;
}
