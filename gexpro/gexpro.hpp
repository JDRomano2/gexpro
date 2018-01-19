#ifndef GEXPRO_HPP
#define GEXPRO_HPP

#include <string>
#include <armadillo>
#include <map>
#include <cassert>

#include <boost/foreach.hpp>

#include "gene.hpp"
#include "sample.hpp"
#include "sigMask.hpp"
#include "normalizer.hpp"

extern int FLAG_VERBOSE;

class GeoParser;

using namespace arma;


typedef enum ExpressionDataType {
  TYPE_LINCS, // Include cmap, clue.io, LINCS l1000...
  TYPE_GTEX,
  TYPE_EBI_GXA,
  TYPE_GEO_DATASET,
  TYPE_GEO_SAMPLE,
  TYPE_GEO_SERIES,
  TYPE_GEO_OTHER,
  TYPE_MISC_OTHER,
} ExpressionDataType;

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

typedef struct SampleGene {
  std::vector<std::string> sample_ids;
  std::vector<std::string> values;
  //std::vector<char> abs_call;
  //std::vector<float> det_pval;
} SampleGene;


template class std::map<std::string,SampleGene>;


class Gexpro {
  // metadata declarations
  std::string profile_name;
  std::string url;

  std::string platformNameStr;
  std::string sampleIdStr;
  std::string seriesStr;

  // data declarations
  // profile data
  std::vector<std::string>* genesIdx;
  std::vector<std::string>* samplesIdx;
  // outer dim: genes. inner dim: samples.
  std::vector<std::vector<std::string>>* raw_data_matrix;

  // sample data
  std::map<std::string,SampleGene> genes;

  fmat data_matrix;

  Normalizer normalizer;
  
public:
  Gexpro(std::string nameStr);
  Gexpro() = default;

  // Setters
  void setPlatformNameStr(const std::string str) { platformNameStr = str; }
  void setSampleIdStr(const std::string str) { sampleIdStr = str; }
  void setSeriesStr(const std::string str) { seriesStr = str; }

  void setRawDataMatrix(std::vector<std::vector<std::string>>* rdm) { raw_data_matrix = rdm; }
  void setDataMatrix(fmat geo) { data_matrix = geo; }

  // Related to Samples
  void addSampleValueToGene(std::string gene_id, std::string sample, std::string value);
  void alignGeneData();

  void printDataHeader(int nrow=5, int ncol=5);
  void dumpMatrix();

  void normalizeFromDataMatrix();
  // TODO:
  // void normalizeFromDataDirectory(params...)

  std::string getName() { return profile_name; }
  int getNSamples() { return samplesIdx->size(); }
  int getNGenes() { return genesIdx->size(); }
  std::vector<std::vector<std::string>>* getRDM() { return raw_data_matrix; }
};

#endif
