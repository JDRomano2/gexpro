#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) : data_matrix(0, 0) {
  profile_name = nameStr;
}

void Gexpro::addSampleValueToGene(std::string gene_id, std::string sample, std::string value) {
  if ( genes.find(gene_id) == genes.end() ) {
    SampleGene sg = { {sample}, {value} };
    genes[gene_id] = sg;
  } else {
    genes[gene_id].sample_ids.push_back(sample);
    genes[gene_id].values.push_back(value);
  }
}

void Gexpro::alignGenes() {
  // In this method, we take the contents of `genes` and build the data matrix
  // This method MUST validate the contents of `genes`.

  // Ensure equal length of vectors in each gene (each gene should be represented exactly once
  // in each sample)
  std::map<std::string,SampleGene>::iterator it;
  it = genes.begin();
  // NOTE: when you iterate over a map, ->first is the key,
  // and ->second is the value
  int nsamples = it->second.sample_ids.size();
  int nvalues = it->second.values.size();
  it++;
  for(it; it != genes.end(); it++) {
    assert (nsamples == it->second.sample_ids.size());
    assert (nvalues == it->second.values.size());
  }

  // Prepare data matrix for contents of `genes`
  // TODO
}

void Gexpro::printDataHeader(int nrow, int ncol) {
  fmat subgeo = data_matrix.submat(0, 0, nrow, ncol);
  std::cout << subgeo << std::endl;
}

void Gexpro::dumpMatrix() {
  std::string extension = ".csv";
  std::string fname_out = profile_name + extension;

  if (FLAG_VERBOSE) {
    std::cout << std::endl;
    std::cout << "NUMBER OF ROWS: " << data_matrix.n_rows << std::endl;
    std::cout << "NUMBER OF COLUMNS: " << data_matrix.n_cols << std::endl;
  }

  data_matrix.save(fname_out, raw_ascii);
}
