#ifndef NORMALIZER_HPP
#define NORMALIZER_HPP

#include <armadillo>
#include <vector>
#include <algorithm>

using namespace arma;


typedef struct NormAggregate {
  unsigned int count;
  fvec mean;
  fvec M2;
} NormAggregate;

typedef struct NormResults {
  fvec mean;
  fvec variance;
  fvec std_dev;
} NormResults;


class Normalizer {
  unsigned int n_genes;
  unsigned int n_seen;

  // Data structures for online normalization algorithm
  NormAggregate agg;
  NormResults res;
public:
  void update(fvec& newValue);
  void finalize();
  void normalizeData(fmat& data);

  Normalizer() = default;

  void initializeWithKnownDimensions(unsigned int ng);

  fvec& getMean() { return res.mean; }
  fvec& getVariance() { return res.variance; }
};

#endif
