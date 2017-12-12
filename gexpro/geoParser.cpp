#include "geoParser.hpp"

Gexpro GeoParser::parseFile(const std::string file_name) {
  // Default profile name: the file name without the extension
  //const std::string file_name = "../../data/geo/GDS1517_full.soft";
  const std::string delim = ".";
  const std::string pro_name = file_name.substr(0, file_name.find(delim));
  std::cout << "Creating expression profile with the name \"" << pro_name << "\"" << std::endl;
  Gexpro gexpr = Gexpro(pro_name);



  return gexpr;
}
