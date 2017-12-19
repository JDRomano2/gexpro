#include "geoParser.hpp"

extern int FLAG_VERBOSE;


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
  
    // Allocate a new gene expression profile
  Gexpro gexpr = Gexpro(pro_name);

  // open file
  current_soft_file.open(file_name);
  ifl = FILE_OPEN;
  if (current_soft_file.good()) {
    if (FLAG_VERBOSE)
      std::cout << "Successfully opened " << file_name << std::endl;
  } else {
    std::cout << "Couldn't open " << file_name << std::endl;
    abort();
  }

  // Iterate over file, parsing line by line
  std::string line;
  while (std::getline(current_soft_file, line)) {
    // get first character

    // ------------------READING DATA---------------------------
    if (reading_data_table == true) {
      if (line[0] == '!') {
	reading_data_table = false;
	this->flushData(gexpr);
	if (FLAG_VERBOSE)
	  std::cout << "Exiting data table read mode..." << std::endl;
	continue;
      }

      // Read a line of data
      data_buffer.push_back(line);
    }

    // ------------------READING METADATA-----------------------
    if (line[0] == '^') {
      // BEGIN ENTITY BLOCK
      this->parseEntityIndicatorLine(gexpr, line);
    } else if (line[0] == '!') {
      // ENTITY ATTRIBUTE
      this->parseEntityAttributeLine(gexpr, line);
    }
    else if (line[0] == '#') {
      // DATA TABLE HEADER DESCRIPTION
      this->parseDataTableHeaderLine(gexpr, line);
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
  if (FLAG_VERBOSE) {
    std::cout << "NEW BLOCK: ";
    std::cout << line << std::endl;
  }
  
  std::istringstream iss(line);
  std::string identifier;
  std::string eqsign;
  std::string remainder;
  iss >> identifier;
  iss >> eqsign;
  iss >> remainder;
  
  if (identifier.compare("^PLATFORM") == 0) {
    current_attribute_block = AttributeBlock::PLATFORM;
    gexpr.setPlatformNameStr(remainder);
  } else if (identifier.compare("^SAMPLE") == 0) {
    current_attribute_block = AttributeBlock::SAMPLE;
    gexpr.setSampleIdStr(remainder);
  } else if (identifier.compare("^SERIES") == 0) {
    current_attribute_block = AttributeBlock::SERIES;
    gexpr.setSeriesStr(remainder);
  } else if (identifier.compare("^DATABASE") == 0) {
    current_attribute_block = AttributeBlock::DATABASE;
    // TODO
  } else if (identifier.compare("^DATASET") == 0) {
    current_attribute_block = AttributeBlock::DATASET;
    // TODO
  } else if (identifier.compare("^SUBSET") == 0) {
    current_attribute_block = AttributeBlock::SUBSET;
    // TODO
  } else if (identifier.compare("^Annotation") == 0) {
    current_attribute_block = AttributeBlock::ANNOTATION;
    // TODO
  } 
}

void GeoParser::parseEntityAttributeLine(Gexpro& gexpr, const std::string line) {

  std::istringstream iss(line);
  std::string identifier;
  std::string eqsign;
  std::string remainder;
  iss >> identifier;
  iss >> eqsign;
  iss >> remainder;

  // branch based on current attribute block
  if (current_attribute_block == AttributeBlock::PLATFORM) {
    
  } else if (current_attribute_block == AttributeBlock::SAMPLE) {

  } else if (current_attribute_block == AttributeBlock::SERIES) {

  } else if (current_attribute_block == AttributeBlock::DATABASE) {
    // For now, skip this block - it just talks about GEO
  } else if (current_attribute_block == AttributeBlock::DATASET) {

    if (FLAG_VERBOSE)
      std::cout << "  DATASET ATTRIBUTE: " << line << std::endl;
    if (line == "!dataset_table_begin") {
      reading_data_table = true;
      return;
    }
    
  } else if (current_attribute_block == AttributeBlock::SUBSET) {

  } else if (current_attribute_block == AttributeBlock::ANNOTATION) {

  }

}

void GeoParser::parseDataTableHeaderLine(Gexpro& gexpr, const std::string line) {
  if (FLAG_VERBOSE)
    std::cout << "  DATA HEADER: " << line << std::endl;

  std::istringstream iss(line);
  std::string identifier;
  std::string eqsign;
  std::string remainder;
  iss >> identifier;
  iss >> eqsign;
  iss >> remainder;
}

void GeoParser::parseDataTableContentLine(Gexpro& gexpr, const std::string line) {

}

void GeoParser::flushData(Gexpro& dest_gexpr) {
  // find dimensions of data in buffer
  int nrows = data_buffer.size() - 1; // first line is just the header
  std::string field_delim = "\t";
  
  std::vector<std::string> tok_header;
  std::string header_line = data_buffer[0];

  // we'll recycle these later in this method
  size_t pos = 0;
  std::string token;
  
  while ((pos = header_line.find(field_delim)) != std::string::npos) {
    token = header_line.substr(0, pos);
    tok_header.push_back(token);
    header_line.erase(0, pos+field_delim.length());
  }

  int ncols = tok_header.size();

  std::vector<std::vector<std::string>> rdm(nrows, std::vector<std::string>(ncols));

  unsigned int i = 0;
  unsigned int j = 0;
  for (std::vector<std::string>::iterator it = data_buffer.begin()+1; it != data_buffer.end(); ++it) {
    size_t last = 0;
    size_t next = 0;
    while ((next = it->find(field_delim, last)) != std::string::npos) {
      // DOES THIS EXTRACT THE LAST TOKEN OF THE STRING?
      // SEE COMMENT ON https://stackoverflow.com/a/14266139/1730417
      rdm[i][j] = it->substr(last, next-last);
      last = next + 1;
      j++;
    }
    j = 0;
    i++;
  }

  dest_gexpr.setRawDataMatrix(&rdm);

  // Initialize the matrix
  fmat geo(45101, 20);

  // Convert numerical data to Armadillo matrix
  for (int i = 0; i < 45101; i++)
    for (int j = 0; j < 20; j++)
      geo(i,j) = std::stof(rdm[i][j+2]);

  dest_gexpr.setDataMatrix(geo);
}
