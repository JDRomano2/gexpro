#ifndef PARSER_HPP
#define PARSER_HPP

#include "gexpro.hpp"

class Parser {
public:
  virtual Gexpro * const parseFile(const std::string file_name) = 0;
};

#endif
