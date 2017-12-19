#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <getopt.h>

#include "gexpro.hpp"
#include "geoParser.hpp"

int FLAG_VERBOSE;
int FLAG_HELP;


int main (int argc, char* argv[]) {
  
  int c;
  std::string pro_fname;

  // Long form for getopt
  while (1) {
    static struct option long_options[] = {
      /* Flags */
      {"verbose", no_argument, &FLAG_VERBOSE, 1},
      {"brief", no_argument, &FLAG_VERBOSE, 0},
      {"help", no_argument, &FLAG_HELP, 1},
      /* non-flag arguments */
      {"infile", required_argument, 0, 'f'},
      {0, 0, 0, 0}
    };
    int option_index = 0;

    c = getopt_long(argc, argv, "f:",
		    long_options, &option_index);

    /* if we've parsed all of the options */
    if (c == -1)
      break;

    switch (c) {
    case 0:
      if (long_options[option_index].flag != 0)
	break;
      std::cout << "option " << long_options[option_index].name;
      if (optarg)
	std::cout << " with arg " << optarg;
      std::cout << std::endl;
      break;

    case 'f':
      std::cout << "Input filename with value: `" << optarg << "'" << std::endl;
      pro_fname = optarg;
      break;

    case '?':
      // We've already printed the error message
      break;

    default:
      return 1;
    }
  }

  if (FLAG_HELP) {
    std::cout << "mkgexpro -- a utility for creating GEXPRO objects" << std::endl;
    return 0;
  }

  GeoParser parser;

  //const std::string pro_fname = "../data/geo/GDS1517_full.soft";
  Gexpro gds1517 = parser.parseFile(pro_fname);

  // Write data matrix to text file
  gds1517.dumpMatrix();
  
  return 0;
}
