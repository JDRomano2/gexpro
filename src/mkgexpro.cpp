#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <future>

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
  // Print user configuration
  std::cout <<                                                                      std::endl;
  std::cout << "+-----------------------------------------------+" <<                   std::endl;
  std::cout << "| MKGEXPRO --- Create GEXPRO files for analysis |" <<                   std::endl;
  std::cout << "+-----------------------------------------------+" <<                   std::endl;
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

  std::cout << vm.count("verbose") << std::endl;
}

/*
 * Return ExpressionDataType enum value for provided accession
 */
ExpressionDataType testAccessionType(std::string id) {

  std::string first3 = id.substr(0,3);

  if      ( first3.compare("GSE") == 0 )
    return TYPE_GEO_SERIES;
  else if ( first3.compare("GDS") == 0 )
    return TYPE_GEO_DATASET;
  else if ( first3.compare("GPL") == 0 )
    return TYPE_GEO_PLATFORM;
  else if ( first3.compare("GSM") == 0 )
    return TYPE_GEO_SAMPLE;
  else
    return TYPE_GEO_OTHER;
}


int main (int argc, char* argv[]) {
  // int i;
  // for (i=0; i<100; i++) {
  //   advance_cursor();
  //   usleep(100000);
  // }
  // printf("\n");

  try {
    std::vector<std::string> accessions;

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
      ("geo-accession(s),G", po::value< std::vector<std::string> >(&accessions)->multitoken(), "Provide an accession string to retrieve from GEO")
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

    GeoParser parser;

    // Get input
    if (accessions.size() > 0) {

      std::cout << "Downloading file..." << std::endl;

      MultiGexpro* mp = new MultiGexpro();

      for (auto& g : accessions) {
        ExpressionDataType et = testAccessionType(g);
        std::cout << "Expression Data Type: " << et << std::endl;
        switch (et) {
        case TYPE_GEO_SERIES:
          {
            Gexpro geosoft = parser.downloadGeoFile(g);
            mp->add(geosoft);
            break;
          }
        case TYPE_GEO_PLATFORM:
          {
            parser.fetchSeriesByPlatform(g);
            break;
          }
        default:
          std::cout << "Error: GEO accession invalid or of unsupported data type" << std::endl;
          std::cout << "Aborting" << std::endl;
          abort();
        }
      }

      std::cout << "Initialized multigexpro" << std::endl;
      mp->findCommonFeatures();
      std::cout << "Found common features" << std::endl;
      mp->removeNonCommonFeatures();
      std::cout << "Removed everything else" << std::endl;

      // std::future<Gexpro> fut = std::async(std::launch::async,
      //                                      &GeoParser::downloadGeoFile,
      //                                      parser,
      //                                      vm["geo-accession"].as<std::string>() );
      // std::chrono::milliseconds span (200);
      // while (fut.wait_for(span) == std::future_status::timeout)
      //   advance_cursor();
      // Gexpro geosoft = fut.get();

      if (vm.count("normalize")) {
        mp->normalizeAllGexpros();
      }

      mp->dumpAllMatrices();
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
