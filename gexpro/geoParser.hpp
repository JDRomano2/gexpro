#ifndef GEO_PARSER_HPP
#define GEO_PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include "gexpro.hpp"

#define GEO_BASE_URL "ftp://ftp.ncbi.nlm.nih.gov/geo/"


typedef enum InputFileLock {
  FILE_NOT_OPEN,
  FILE_OPEN,
} InputFileLock;

// FOR THE FOLLOWING, REFER TO:
// https://www.ncbi.nlm.nih.gov/geo/info/overview.html
typedef enum class AttributeBlock {
  OUT_OF_BLOCK,  // maintain this state while not parsing
  DATABASE,      // information regarding GEO in general
  PLATFORM,      // metadata describing analysis platform (e.g., "Illumina HiSeq 2500 ...")
  SAMPLE,        // metadata describing individual samples
  SERIES,        // metadata describing an experiment consisting of one or more samples
  DATASET,       // multiple SERIES assembled by GEO curators
  SUBSET,        // group of SAMPLES within a DATASET associated with a major experimental variable
  ANNOTATION,    // metadata describing the protocol used to annotate genes
} AttributeBlock;


class GeoParser {
  std::ifstream current_soft_file;
  InputFileLock ifl = FILE_NOT_OPEN;
  AttributeBlock current_attribute_block;
  bool reading_data_table = false;
  std::vector<std::string> data_buffer;

public:
  // Top-level parsing functions
  Gexpro parseFile(const std::string file_name);
  Gexpro parseFile(const boost::iostreams::filtering_istream* gzstream, std::string proname);
  Gexpro downloadGeoFile(const std::string id);

  // Utility functions
  ExpressionDataType detectDataType(std::string fnameOrURL) { return TYPE_GEO_SERIES };  // need to implement

  // Parse components of a SOFT file
  void parseEntityIndicatorLine(Gexpro& gexpr, const std::string line);
  void parseEntityAttributeLine(Gexpro& gexpr, const std::string line);
  void parseDataTableHeaderLine(Gexpro& gexpr, const std::string line);
  void parseDataTableContentLine(Gexpro& gexpr, const std::string line);

  // Misc
  void flushData(Gexpro& dest_gexpr);

  // Constructors
  GeoParser() = default;
};

#endif
