#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) : data_matrix(0, 0) {
  profile_name = nameStr;
}

void Gexpro::printDataHeader(int nrow, int ncol) {
  fmat subgeo = data_matrix.submat(0, 0, nrow, ncol);
  std::cout << subgeo << std::endl;
}

void Gexpro::dumpMatrix() {
  // First iteration: just write to csv file
  std::string fname_out = "csv_file.csv";

  if (FLAG_VERBOSE) {
    std::cout << std::endl;
    std::cout << "NUMBER OF ROWS: " << data_matrix.n_rows << std::endl;
    std::cout << "NUMBER OF COLUMNS: " << data_matrix.n_cols << std::endl;
  }

  data_matrix.save(fname_out, raw_ascii);
}
