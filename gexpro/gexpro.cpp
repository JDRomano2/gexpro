#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) {
  profile_name = nameStr;
}

std::string Gexpro::getName() {
  return profile_name;
}
