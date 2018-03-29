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
//#include "gexproConfig.hpp"  <-- can't get this to work...

extern int FLAG_VERBOSE;

class GeoParser;

using namespace arma;

#define Gexpro_VERSION_MAJOR  1
#define Gexpro_VERSION_MINOR  0


typedef enum ExpressionDataType {
  TYPE_LINCS, // Include cmap, clue.io, LINCS l1000...
  TYPE_GTEX,
  TYPE_EBI_GXA,
  TYPE_GEO_DATASET,
  TYPE_GEO_SAMPLE,
  TYPE_GEO_SERIES,
  TYPE_GEO_PLATFORM,
  TYPE_GEO_OTHER,
  TYPE_MISC_OTHER,
} ExpressionDataType;

// The default unit for each feature's expression value
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
  int n_features;
  int n_samples;
} ProfileDims;

typedef struct SampleValue {
  std::vector<std::string> sample_ids;
  std::vector<std::string> values;
  //std::vector<char> abs_call;
  //std::vector<float> det_pval;
} SampleValue;


template class std::map<std::string,SampleValue>;


class Gexpro {
  // metadata declarations
  std::string profile_name;
  std::string url;

  std::string platformNameStr;
  std::string sampleIdStr;
  std::string seriesStr;

  // data declarations
  // profile data
  std::vector<std::string> featuresIdx;
  std::vector<std::string> samplesIdx;
  // outer dim: featuress. inner dim: samples.
  std::vector<std::vector<std::string>>* raw_data_matrix;

  // sample data
  std::map<std::string,SampleValue> features;

  // COLUMNS: SAMPLES
  // ROWS:    FEATURES
  fmat data_matrix;

  Normalizer normalizer;
  
public:
  Gexpro() = default;
  Gexpro(std::string nameStr);
  
  // Setters
  void setPlatformNameStr(const std::string str) { platformNameStr = str; }
  void setSampleIdStr(const std::string str) { sampleIdStr = str; }
  void setSeriesStr(const std::string str) { seriesStr = str; }

  void setRawDataMatrix(std::vector<std::vector<std::string>>* rdm) { raw_data_matrix = rdm; }
  void setDataMatrix(fmat geo) { data_matrix = geo; }

  // Other mutators
  void removeFeatureByName(std::string feat, bool fail_silently = false);
  void removeFeatureByIndex(int f_ind);

  // Related to Samples
  void addSampleValueToFeature(std::string feature_id, std::string sample, std::string value);
  void alignFeatureData();

  void printDataHeader(int nrow=5, int ncol=5);
  void dumpMatrix(std::string fname_prefix);

  void normalizeFromDataMatrix();
  // TODO:
  // void normalizeFromDataDirectory(params...)

  std::string getName() const { return profile_name; }
  int getNSamples() { return samplesIdx.size(); }
  int getNFeatures() { return featuresIdx.size(); }
  int getNDataCols() { return data_matrix.n_cols; }
  int getNDataRows() { return data_matrix.n_rows; }
  std::vector<std::string> getFeatureNames() { return featuresIdx; }
  std::vector<std::string> getSampleNames() const { return samplesIdx; }
  std::vector<std::vector<std::string>>* getRDM() { return raw_data_matrix; }
};

#endif
