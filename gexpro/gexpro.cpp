#include "gexpro.hpp"

Gexpro::Gexpro(std::string nameStr) : data_matrix(0, 0) {
  profile_name = nameStr;
}

void Gexpro::addSampleValueToGene(std::string gene_id, std::string sample, std::string value) {
  if ( genes.find(gene_id) == genes.end() ) {
    genes[gene_id] = { {sample}, {value} };
  } else {
    genes[gene_id].sample_ids.emplace_back(sample);
    genes[gene_id].values.emplace_back(value);
  }
}

void Gexpro::alignGeneData() {
  // In this method, we take the contents of `genes` and build the data matrix
  // This method MUST validate the contents of `genes`.
  
  std::cout << "Beginning to align data in GEXPRO object." << std::endl;
  std::pair<std::string, SampleGene> me;
  std::vector<std::string> gidx;
  std::vector<std::string> sidx = genes.begin()->second.sample_ids;
  BOOST_FOREACH(me, genes) {
    gidx.emplace_back(me.first);
  }
  std::cout << "Total number of genes: " << gidx.size() << std::endl;

  std::vector<std::vector<std::string>> rdm(gidx.size(), std::vector<std::string>(sidx.size()));

  // Ensure equal length of vectors in each gene (each gene should be represented exactly once
  // in each sample)
  //TODO

  int i = 0;
  for (auto const& g_name : gidx) {
    std::cout << g_name << std::endl;
    rdm[i] = genes[g_name].values;
    i++;
  }

  std::cout << "NUMBER OF SAMPLES: " << sidx.size() << std::endl;
  std::cout << "NUMBER OF GENES:   " << gidx.size() << std::endl;

  samplesIdx = &sidx;
  genesIdx = &gidx;

  this->setRawDataMatrix(&rdm);

  for(int i = 0; i < sidx.size(); i++) {
    std::cout << (*raw_data_matrix)[0][i] << std::endl;
  }

  fmat geo(gidx.size(), sidx.size());

  for(int i = 0; i < gidx.size(); i++)
    for (int j = 0; j < sidx.size(); j++)
      geo(i,j) = std::stof(rdm[i][j]);

  this->setDataMatrix(geo);
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
    std::cout << "Saving output to: " << fname_out << std::endl;
  }

  data_matrix.save(fname_out, raw_ascii);
}
