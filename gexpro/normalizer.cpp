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
