#include "geoParser.hpp"

constexpr unsigned int str2int(const char* str, int h = 0) {
  // see: https://stackoverflow.com/a/16388610/1730417
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

Gexpro GeoParser::parseFile(const std::string file_name) {
  // Default profile name: the file name without the extension
  const std::string delim = ".";
  std::size_t found = file_name.find_last_of("/\\");
  std::string fname_no_path = file_name.substr(found+1);
  const std::string pro_name = fname_no_path.substr(0, fname_no_path.find(delim));
  
  std::cout << "Creating expression profile with the name \"" << pro_name << "\"" << std::endl;

  // Allocate a new gene expression profile
  Gexpro gexpr = Gexpro(pro_name);

  // open file
  current_soft_file = std::ifstream(file_name);
  ifl = FILE_OPEN;
  if (current_soft_file.good())
    std::cout << "Successfully opened " << file_name << std::endl;
  else
    std::cout << "Couldn't open " << file_name << std::endl;

  // Iterate over file, parsing line by line
  std::string line;
  while (std::getline(current_soft_file, line)) {
    // get first character

    if (line[0] == '^') {
      // BEGIN ENTITY BLOCK
      GeoParser::parseEntityIndicatorLine(gexpr, line);
    } else if (line[0] == '!') {
      // ENTITY ATTRIBUTE
      GeoParser::parseEntityAttributeLine(gexpr, line);
    }
    else if (line[0] == '#') {
      // DATA TABLE HEADER DESCRIPTION
      continue;
    } else {
      // DATA TABLE CONTENT
      continue;
    }
  }
  current_soft_file.close();
  ifl = FILE_NOT_OPEN;


  return gexpr;
}

void GeoParser::parseEntityIndicatorLine(Gexpro& gexpr, const std::string line) {
  std::cout << "Parsing entity indicator:" << std::endl;
  std::cout << "  " << line << std::endl;
  
  std::istringstream iss(line);
  std::string identifier;
  std::string eqsign;
  std::string remainder;
  iss >> identifier;
  iss >> eqsign;
  iss >> remainder;
  
  if (identifier.compare("^PLATFORM")) {
    current_attribute_block = PLATFORM;
    gexpr.setPlatformNameStr(remainder);
  } else if (identifier.compare("^SAMPLE")) {
    current_attribute_block = SAMPLE;
    gexpr.setSampleIdStr(remainder);
  } else if (identifier.compare("^SERIES")) {
    current_attribute_block = SERIES;
    gexpr.setSeriesStr(remainder);
  } else if (identifier.compare("^DATABASE")) {
    current_attribute_block = DATABASE;
    // TODO
  } else if (identifier.compare("^DATASET")) {
    current_attribute_block = DATASET;
    // TODO
  } else if (identifier.compare("^SUBSET")) {
    current_attribute_block = SUBSET;
    // TODO
  } else if (identifier.compare("^ANNOTATION")) {
    current_attribute_block = ANNOTATION;
    // TODO
  } 
}

void GeoParser::parseEntityAttributeLine(Gexpro& gexpr, const std::string line) {
  std::cout << "Parsing entity attribute:" << std::endl;
  std::cout << "  " << line << std::endl;

  std::istringstream iss(line);
  std::string identifier;
  std::string eqsign;
  std::string remainder;
  iss >> identifier;
  iss >> eqsign;
  iss >> remainder;

}

void GeoParser::parseDataTableHeaderLine(Gexpro& gexpr, const std::string line) {

}

void GeoParser::parseDataTableContentLine(Gexpro& gexpr, const std::string line) {

}
