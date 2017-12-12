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


class GeoParser {
  std::ifstream current_soft_file;
  InputFileLock ifl = FILE_NOT_OPEN;
public:
  Gexpro parseFile(const std::string file_name);

  static void parseEntityIndicatorLine(Gexpro& gexpr, const std::string line);
  static void parseEntityAttributeLine(Gexpro& gexpr, const std::string line);
  static void parseDataTableHeaderLine(Gexpro& gexpr, const std::string line);
  static void parseDataTableContentLine(Gexpro& gexpr, const std::string line);

  GeoParser() = default;
};

#endif
