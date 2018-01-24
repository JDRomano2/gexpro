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


void validateOptions(po::options_description& all_opts, po::variables_map& vm) {
  // Print user configuration
  std::cout << std::endl;
  std::cout << "GEXPRO CONFIGURATION" << std::endl;
  std::cout << "version " << Gexpro_VERSION_MAJOR << "." << Gexpro_VERSION_MINOR << std::endl;
  std::cout << std::endl;
  std::cout << "VERBOSE: " << (bool)(vm.count("verbose")) << std::endl;

  std::cout << std::endl;

  // Validate
  if (vm.count("geo-accession") && vm.count("geo-file")) {
    std::cout << "ERROR: " << std::endl;
    std::cout << "Specify either GEO accession or GEO file, not both." << std::endl;
  }
  if (!(vm.count("geo-accession")) && !(vm.count("geo-file"))) {
    std::cout << "ERROR: " << std::endl;
    std::cout << "Must provide either a GEO accession or a local path to a GEO SOFT file." << std::endl;
  }
}


int main (int argc, char* argv[]) {

  try {
    // Specify command-line options
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

    // Assemble the option groups and parse user-provided options
    po::options_description all("Allowed options");
    all.add(general_opts).add(io_opts).add(data_opts);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, all), vm);
    po::notify(vm);

    // Handle informational options
    if (vm.count("help")) {
      std::cout << all << std::endl;
      return 0;
    }
    if (vm.count("version")) {
      std::cout << "GEXPRO VERSION: "
		<< Gexpro_VERSION_MAJOR
		<< "."
		<< Gexpro_VERSION_MINOR
		<< std::endl;
      return 0;
    }
    if (vm.count("quiet")) {
      // disable console output via cout
      std::cout.setstate(std::ios_base::failbit);
    }

    // Validate arguments
    validateOptions(all, vm);

    // Handle program logic

    // Instantiate objects
    GeoParser parser;

    // Get input
    if (vm.count("geo-accession")) {
      // test data type
      Gexpro geosoft = parser.downloadGeoFile( vm["geo-accession"].as<std::string>() );

      if (vm.count("normalize")) {
        geosoft.normalizeFromDataMatrix();
      }
      // write matrix to file
      geosoft.dumpMatrix( vm["geo-accession"].as<std::string>() );
    }


    if (vm.count("quiet")) {
      // reenable console output via cout
      std::cout.clear();
    }

  } catch(std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
