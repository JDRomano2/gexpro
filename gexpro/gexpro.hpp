#ifndef GEXPRO_HPP
#define GEXPRO_HPP

#include <string>
#include <armadillo>

#include "gene.hpp"
#include "sample.hpp"
#include "sigMask.hpp"

using namespace arma;


typedef enum ProfileSource {
  SOURCE_LINCS, // Include cmap, clue.io, LINCS l1000...
  SOURCE_GTEX,
  SOURCE_EBI_GXA,
  SOURCE_GEO_OTHER,
  SOURCE_MISC_OTHER,
} ProfileSource;


// The default unit for each gene's expression value
typedef enum ExprBaseUnit {
  UNIT_COUNT,
  UNIT_Z_SCORE
} ExprBaseUnit;


typedef enum PlatformClass {
  PLATFORM_MICROARRAY,
  PLATFORM_RNA_SEQ,
  PLATFORM_QPCR,
} PlatformClass;


typedef struct ProfileDims {
  int n_genes;
  int n_samples;
} ProfileDims;


class Gexpro {
  // metadata declarations
  std::string profile_name;
  std::string url;
  ProfileSource source;
  ExprBaseUnit base_unit;
  int n_samples;
  int n_genes;

  // data declarations
  SpMat<double> &count_matrix;
  Col<Gene> &genes;
  Row<Sample> &samples;
  SignificanceMask &mask;

  // statistic declarations
  double sparsity;
  ProfileDims dim;
  
public:
  Gexpro(std::string nameStr);
  
  std::string getName();
};

#endif