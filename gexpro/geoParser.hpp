#ifndef GEO_PARSER_HPP
#define GEO_PARSER_HPP

#include "gexpro.hpp"
//#include "parser.hpp"

class GeoParser {
public:
  Gexpro parseFile(const std::string file_name);
  //Gexpro parseFile();

  GeoParser() = default;
};

#endif
