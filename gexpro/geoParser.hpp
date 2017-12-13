#ifndef GEO_PARSER_HPP
#define GEO_PARSER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstddef>

#include "gexpro.hpp"
//#include "parser.hpp"

typedef enum InputFileLock {
  FILE_NOT_OPEN,
  FILE_OPEN,
} InputFileLock;

// FOR THE FOLLOWING, REFER TO:
// https://www.ncbi.nlm.nih.gov/geo/info/overview.html
typedef enum AttributeBlock {
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
public:
  Gexpro parseFile(const std::string file_name);

  void parseEntityIndicatorLine(Gexpro& gexpr, const std::string line);
  void parseEntityAttributeLine(Gexpro& gexpr, const std::string line);
  void parseDataTableHeaderLine(Gexpro& gexpr, const std::string line);
  void parseDataTableContentLine(Gexpro& gexpr, const std::string line);

  GeoParser() = default;
};

#endif
