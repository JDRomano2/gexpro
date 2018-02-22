#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) : data_matrix(0, 0) {
  profile_name = nameStr;
}

void Gexpro::addSampleValueToFeature(std::string feature_id, std::string sample, std::string value) {
  if ( features.find(feature_id) == features.end() ) {
    features[feature_id] = { {sample}, {value} };
  } else {
    features[feature_id].sample_ids.emplace_back(sample);
    features[feature_id].values.emplace_back(value);
  }
}

void Gexpro::alignFeatureData() {
  // In this method, we take the contents of `features` and build the data matrix
  // This method MUST validate the contents of `features`.

  std::pair<std::string, SampleValue> me;
  std::vector<std::string> gidx;
  std::vector<std::string> sidx = features.begin()->second.sample_ids;
  BOOST_FOREACH(me, features) {
    gidx.emplace_back(me.first);
  }
  std::cout << "Total number of features: " << gidx.size() << std::endl;

  std::vector<std::vector<std::string>> rdm(gidx.size(), std::vector<std::string>(sidx.size()));

  // Ensure equal length of vectors in each feature (each feature should be represented exactly once
  // in each sample)
  //TODO

  int i = 0;
  for (auto const& g_name : gidx) {
    rdm[i] = features[g_name].values;
    i++;
  }

  samplesIdx = &sidx;
  featuresIdx = gidx;

  this->setRawDataMatrix(&rdm);

  // Print values for first feature
  // for(size_t i = 0; i < sidx.size(); i++) {
  //   std::cout << (*raw_data_matrix)[0][i] << std::endl;
  // }

  fmat geo(gidx.size(), sidx.size());

  for(size_t i = 0; i < gidx.size(); i++)
    for (size_t j = 0; j < sidx.size(); j++)
      geo(i,j) = std::stof(rdm[i][j]);

  this->setDataMatrix(geo);
}

void Gexpro::printDataHeader(int nrow, int ncol) {
  fmat subgeo = data_matrix.submat(0, 0, nrow, ncol);
  //std::cout << subgeo << std::endl;
}

void Gexpro::dumpMatrix(std::string fname_prefix) {
  std::string extension = ".csv";
  std::string fname_out = fname_prefix + extension;

  if (FLAG_VERBOSE) {
    std::cout << std::endl;
    std::cout << "NUMBER OF ROWS: " << data_matrix.n_rows << std::endl;
    std::cout << "NUMBER OF COLUMNS: " << data_matrix.n_cols << std::endl;
    std::cout << "Saving output to: " << fname_out << std::endl;
  }

  data_matrix.save(fname_out, csv_ascii);
}

void Gexpro::normalizeFromDataMatrix() {
  normalizer.initializeWithKnownDimensions( data_matrix.n_rows );
  std::cout << "Finding mean and variance for each feature..." << std::endl;
  data_matrix.each_col( [this](fvec& a){ normalizer.update(a); } );
  normalizer.finalize();
  std::cout << "  ...done." << std::endl;
  //std::cout << "Mean:     " << normalizer.getMean() << std::endl;
  //std::cout << "Variance: " << normalizer.getVariance() << std::endl;

  normalizer.normalizeData(data_matrix);
}
