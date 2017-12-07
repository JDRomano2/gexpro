#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr,
	       SpMat<double> &count_matrix,
	       Col<Gene> &genes,
	       Row<Sample> &samples,
	       SignficanceMask &mask) : nameStr(nameStr), count_matrix(count_matrix), genes(genes), samples(samples), mask(mask) {}

std::string Gexpro::getName() {
  return name;
}
