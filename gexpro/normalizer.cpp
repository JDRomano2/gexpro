#include "normalizer.hpp"


void Normalizer::update(fvec& newValue) {
  agg.count++;
  fvec delta = newValue - agg.mean;
  agg.mean = agg.mean + (delta / agg.count);
  fvec delta2 = newValue - agg.mean;
  agg.M2 = agg.M2 + (delta % delta2);
}

void Normalizer::finalize() {
  fvec variance = agg.M2 / (agg.count - 1);
  res.mean = agg.mean;
  if (agg.count < 2)
    res.variance.fill(datum::nan);
  else
    res.variance = variance;
  res.std_dev = sqrt(res.variance);
}

void Normalizer::normalizeData(fmat& data) {
  // Make sure we have computed mean and variance
  // TODO

  std::cout << "Gene 1, sample 1: " << data(0,0) << std::endl;

  std::cout << "Normalizing each gene's values..." << std::endl;
  data.each_col( [this](fvec& c) { c = ((c - res.mean)/res.std_dev) ;} );
  std::cout << "  ...done." << std::endl;

  std::cout << "Gene 1 mean: " << res.mean[0] << std::endl;
  std::cout << "Gene 1 variance: " << res.variance[0] << std::endl;
  std::cout << "Gene 1 stddev: " << res.std_dev[0] << std::endl;
  std::cout << "Gene 1, sample 1 normalized: " << data(0,0) << std::endl;
}

void Normalizer::initializeWithKnownDimensions(unsigned int ng) {
  n_genes = ng;
  n_seen = 0;

  agg = {
    0,
    fvec(ng,1),
    fvec(ng,1)
  };

  res = {
    fvec(ng,1),
    fvec(ng,1)
  };
}
