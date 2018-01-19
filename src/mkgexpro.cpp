#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <getopt.h>

#include <archive.h>

#include "gexpro.hpp"
#include "geoParser.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int FLAG_VERBOSE;
int FLAG_HELP;

std::string ver_msg = "GEXPRO VERSION: 0.1a";


int main (int argc, char* argv[]) {

  try {
    po::options_description general_opts("General options");
    general_opts.add_options()
      ("help,h", "Show this help message")
      ("version", "Display GEXPRO library version and exit")
      ("verbose,v", "Enable verbose console output")
      ("quiet,q", "Suppress all console output (overrides verbose option)")
      ;

    po::options_description io_opts("Input/output options");
    io_opts.add_options()
      ("geo-accession,G", po::value<std::string>(), "Provide an accession string to retrieve from GEO")
      ("geo-file,g", po::value<std::string>(), "Load a GEO SOFT file from a local directory")
      ("output,o", po::value<std::string>(), "Set output directory for created file(s)")
      ;

    po::options_description data_opts("Data processing options");
    data_opts.add_options()
      ("normalize,n", "Normalize raw values by the best method available")
      ;

    po::options_description all("Allowed options");
    all.add(general_opts).add(io_opts).add(data_opts);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << all << std::endl;
      return 0;
    }
    if (vm.count("version")) {
      std::cout << ver_msg << std::endl;
      return 0;
    }

    if (vm.count("quiet")) {
      std::cout.setstate(std::ios_base::failbit);
    }


    
    GeoParser parser;

    //const std::string pro_fname = "../data/geo/GDS1517_full.soft";
    //Gexpro gds1517 = parser.parseFile(pro_fname);
    const std::string pro_id = "GSE3678";
    Gexpro gds1517 = parser.downloadGeoFile(pro_id);

    // Test normalization
    gds1517.normalizeFromDataMatrix();

    // Write data matrix to text file
    gds1517.dumpMatrix();


    
    if (vm.count("quiet")) {
      std::cout.clear();
    }
    
  } catch(std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}
