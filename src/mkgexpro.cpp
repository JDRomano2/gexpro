#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <getopt.h>
#include <future>

#include <archive.h>

#include "gexpro.hpp"
#include "multiGexpro.hpp"
#include "geoParser.hpp"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int FLAG_VERBOSE;
int FLAG_HELP;

void advance_cursor() {
  static int pos=0;
  char cursor[4] = {'/','-','\\','|'};
  printf("%c\b", cursor[pos]);
  fflush(stdout);
  pos = (pos+1) % 4;
}

void validateOptions(po::options_description& all_opts, po::variables_map& vm) {
  //std::cout << std::boolalpha;  // Display 'true/false' instead of '1/0' for bool

  // Print user configuration
  std::cout <<                                                                      std::endl;
  std::cout << "---------------------------------------------" <<                   std::endl;
  std::cout << "MKGEXPRO --- Create GEXPRO files for analysis" <<                   std::endl;
  std::cout << "---------------------------------------------" <<                   std::endl;
  std::cout                                                                      << std::endl;
  std::cout << "Copyright (c) 2018 by Joe Romano and the Tatonetti Lab."         << std::endl;
  std::cout <<                                                                      std::endl;
  std::cout << "version " << Gexpro_VERSION_MAJOR << "." << Gexpro_VERSION_MINOR << std::endl;
  std::cout <<                                                                      std::endl;
  std::cout << "===============================================================" << std::endl;
  std::cout << "= CONFIGURATION OPTIONS                                       =" << std::endl;
  std::cout << "===============================================================" << std::endl;
  std::cout << "VERBOSE OUTPUT: " << (bool)(vm.count("verbose")) <<                 std::endl;
  std::cout << "NORMALIZE TRANSCRIPT VALUES: " << (bool)(vm.count("normalize")) <<  std::endl;
  std::cout <<                                                                      std::endl;

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
  // int i;
  // for (i=0; i<100; i++) {
  //   advance_cursor();
  //   usleep(100000);
  // }
  // printf("\n");

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
      std::cout << "Downloading file..." << std::endl;

      Gexpro geosoft = parser.downloadGeoFile( vm["geo-accession"].as<std::string>() );
      Gexpro geosoft2 = parser.downloadGeoFile( "GSE110312" );
      Gexpro& gsr = geosoft;
      Gexpro& gsr2 = geosoft2;
      //std::cout << "Double check number of features: " << gsr.getFeatureNames()[0] << std::endl;
      MultiGexpro* mp = new MultiGexpro(gsr);
      mp->add(gsr2);
      std::cout << "Initialized multigexpro" << std::endl;
      mp->findCommonFeatures();
      std::cout << "Number of features in all samples: ";
      std::cout << mp->numCommonFeatures() << std::endl;
      // std::future<Gexpro> fut = std::async(std::launch::async,
      //                                      &GeoParser::downloadGeoFile,
      //                                      parser,
      //                                      vm["geo-accession"].as<std::string>() );
      // std::chrono::milliseconds span (200);
      // while (fut.wait_for(span) == std::future_status::timeout)
      //   advance_cursor();
      // Gexpro geosoft = fut.get();

      if (vm.count("normalize")) {
        geosoft.normalizeFromDataMatrix();
      }
      // write matrix to file
      geosoft.dumpMatrix( vm["geo-accession"].as<std::string>() );
      geosoft2.dumpMatrix( "GSE110312" );
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
