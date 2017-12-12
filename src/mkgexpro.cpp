#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "gexpro.hpp"
//#include "parser.hpp"
#include "geoParser.hpp"

int main (int argc, char *argv[]) {

  GeoParser parser;

  //std::string gexproname = "test";
  //Gexpro *g = new Gexpro(gexproname);  
  //std::cout << "Your Gexpro instance is named " << g->getName() << std::endl;

  const std::string testpro_fname = "GDS1517_full.soft";
  Gexpro gds1517 = parser.parseFile(testpro_fname);
  //Gexpro gds1517 = parser.parseFile();

  std::cout << "Your profile is named " << gds1517.getName() << std::endl;
  
  return 0;
}
