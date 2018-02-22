#include "geoParser.hpp"

extern int FLAG_VERBOSE;


size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written = fwrite(ptr, size, nmemb, stream);
  return written;
}

constexpr unsigned int str2int(const char* str, int h = 0) {
  // see: https://stackoverflow.com/a/16388610/1730417
  return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

Gexpro GeoParser::parseFile(const std::string file_name) {
  // Given a file_name, fill the current Gexpro object with that file's contents

  //std::cout << "Trying to open file: " << file_name << std::endl;;
  
  // Default profile name: the file name without the extension
  const std::string delim = ".";
  std::size_t found = file_name.find_last_of("/\\");
  std::string fname_no_path = file_name.substr(found+1);
  const std::string pro_name = fname_no_path.substr(0, fname_no_path.find(delim));
  
    // Allocate a new expression profile
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
    if (reading_dataset_table == true) {
      if (line[0] == '!') {
	reading_dataset_table = false;
	this->flushDataset(gexpr);
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

Gexpro GeoParser::downloadGeoFile(const std::string id) {
  Gexpro downloaded;

  // recognize type of file
  std::string first3 = id.substr(0,3);
  std::string last3 = id.substr(id.length()-3,3);
  std::string between = id.substr(3,id.length()-6);

  ExpressionDataType edt;
  std::string url;
  std::string fname = std::string(id)+".soft.gz";

  if      ( first3.compare("GSE") == 0 )
    edt = TYPE_GEO_SERIES;
  else if ( first3.compare("GDS") == 0 )
    edt = TYPE_GEO_DATASET;
  else if ( first3.compare("GSM") == 0 )
    edt = TYPE_GEO_SAMPLE;
  else
    edt = TYPE_GEO_OTHER;

  switch (edt) {
  case TYPE_GEO_SERIES:
    url = std::string(GEO_BASE_URL)+"series/GSE"+between+"nnn/"+id+"/soft/"+id+"_family.soft.gz";
    break;
  case TYPE_GEO_DATASET:
    break;
  case TYPE_GEO_SAMPLE:
    break;
  case TYPE_GEO_OTHER:
    break;
  default:
    std::cout << "Error: Unable to detect sample type." << std::endl;
    abort();
  }

  // DOWNLOAD THE FILE
  // combination of two code samples:
  // https://stackoverflow.com/a/1636415/1730417
  // http://en.cppreference.com/w/cpp/io/c/tmpfile
  CURL *curl;
  FILE *fp;
  CURLcode res;
  const char *c_url = url.c_str();
  char outfilename[FILENAME_MAX];
  strcpy(outfilename, fname.c_str());
  curl = curl_easy_init();
  if (curl) {
    fp = fopen(outfilename, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, c_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);
  }

  // PARSE THE FILE
  // See: https://stackoverflow.com/a/6421029/1730417
  std::ifstream file(fname, std::ios_base::in | std::ios_base::binary);
  try {
    std::cout << "Successfully downloaded file from URL " << url << std::endl;
    std::cout << "Decompressing..." << std::endl;
    boost::iostreams::filtering_istream in;
    in.push(boost::iostreams::gzip_decompressor());
    in.push(file);
    std::string str;
    std::cout << "  ...completed" << std::endl;
    //downloaded = parseFile(in, id);

    // Iterate over file, parsing line by line
    std::string line;
    for (line; std::getline(in, line); ) {
      // get first character

      // ------------------READING SAMPLE-----------------------------
      if (reading_sample_table == true) {
	if (line[0] == '!') {
	  reading_sample_table = false;
	  this->flushSample(downloaded);
	  if (FLAG_VERBOSE)
	    std::cout << "Finished reading sample data, continuing..." << std::endl;
	  continue;
	}

	data_buffer.push_back(line);
      }

      // ------------------READING DATABASE---------------------------
      if (reading_dataset_table == true) {
	if (line[0] == '!') {
	  reading_dataset_table = false;
	  this->flushDataset(downloaded);
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
	this->parseEntityIndicatorLine(downloaded, line);
      } else if (line[0] == '!') {
	// ENTITY ATTRIBUTE
	this->parseEntityAttributeLine(downloaded, line);
      }
      else if (line[0] == '#') {
	// DATA TABLE HEADER DESCRIPTION
	this->parseDataTableHeaderLine(downloaded, line);
      } else {
	// DATA TABLE CONTENT
	continue;
      }
    }
    //current_soft_file.close();
    ifl = FILE_NOT_OPEN;
    
  } catch(const boost::iostreams::gzip_error& e) {
    std::cout << e.what() << '\n';
  }

  downloaded.alignFeatureData();

  return downloaded;
}

void GeoParser::parseEntityIndicatorLine(Gexpro& gexpr, const std::string line) {
  // if (FLAG_VERBOSE) {
  //   std::cout << "NEW BLOCK: ";
  //   std::cout << line << std::endl;
  // }
  std::cout << "NEW BLOCK: ";
  std::cout << line << std::endl;
  
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
    
    if (FLAG_VERBOSE)
      std::cout << "  SAMPLE ATTRIBUTE: " << line << std::endl;
    if (line == "!sample_table_begin") {
      reading_sample_table = true;
      return;
    } else if (line == "!sample_table_end") {
      // Flush data
    }
    
  } else if (current_attribute_block == AttributeBlock::SERIES) {
    
  } else if (current_attribute_block == AttributeBlock::DATABASE) {
    // For now, skip this block - it just talks about GEO
  } else if (current_attribute_block == AttributeBlock::DATASET) {

    if (FLAG_VERBOSE)
      std::cout << "  DATASET ATTRIBUTE: " << line << std::endl;
    if (line == "!dataset_table_begin") {
      reading_dataset_table = true;
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

void GeoParser::flushDataset(Gexpro& dest_gexpr) {
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

  // Holds the raw (string) data
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

  // Convert (string) data to Armadillo matrix (float)
  for (int i = 0; i < 45101; i++)
    for (int j = 0; j < 20; j++)
      geo(i,j) = std::stof(rdm[i][j+2]);

  dest_gexpr.setDataMatrix(geo);
}

void GeoParser::flushSample(Gexpro& dest_gexpr) {
  int nrows = data_buffer.size() - 1;
  std::string field_delim = "\t";

  size_t pos = 0;
  std::string token;

  // Iterate over each row of data for the Sample
  for (std::vector<std::string>::iterator it = data_buffer.begin()+1; it != data_buffer.end(); ++it) {
    // get the first (id) and second (value) tokens
    std::string current_sample_id = "TEST";
    
    std::string current_row = *it;
    std::istringstream iss(current_row);
    std::vector<std::string> tokens;
    copy(std::istream_iterator<std::string>(iss),
	 std::istream_iterator<std::string>(),
	 std::back_inserter(tokens));
    // create feature if needed, otherwise append to it
    dest_gexpr.addSampleValueToFeature(tokens[0], current_sample_id, tokens[1]);
  }

  // Reset data buffer
  data_buffer.clear();
}
