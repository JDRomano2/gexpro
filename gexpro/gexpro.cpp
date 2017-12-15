#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) {
  profile_name = nameStr;
}

void Gexpro::printDataHeader(int nrow, int ncol) {
  fmat subgeo = data_matrix->submat(0, 0, nrow, ncol);
  std::cout << subgeo << std::endl;
}
