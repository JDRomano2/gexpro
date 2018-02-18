#include "multiGexpro.hpp"

MultiGexpro::MultiGexpro(const Gexpro& firstGexpro) {
  // Unfortunately we can't make a vector of references; store instead as vector of pointers
  // (perhaps reimplement as boost::ptr_vector)
  const Gexpro *gp = &firstGexpro;
  gps = { gp };
}

const Gexpro* MultiGexpro::getGpByIndex(int gp_index) {
  const Gexpro* gp_ref = (gps[gp_index]);
  return gp_ref;
}

const Gexpro* MultiGexpro::getGpByName(std::string gp_name) {
  const Gexpro* gp_ptr;
  bool found_gp = false;
  for(auto const gp: gps) {
    if ( gp->getName().compare(gp_name) == 0) {
      if (found_gp == false) {
        gp_ptr = gp;
        found_gp = true;
      } else {
        throw std::out_of_range( "Error: More than one GEXPRO with same name!" );
        gp_ptr = NULL;
        return gp_ptr;
      }
    }
  }

  return gp_ptr;
}

void MultiGexpro::add(Gexpro& gp) {
  // Add new pointer to Gexpro instance to gps
  gps.push_back(&gp);
}

void MultiGexpro::remove(Gexpro& gp) {
  gps.erase(std::remove(gps.begin(),gps.end(), &gp), gps.end());
}

/*!
 * Find the features shared by all Gexpro elements in `gps`.
 * Currently, this is not supported for cross-technology analyses,
 * like RNA-Seq and microarray data, but we will explore possible approaches
 * to do this in the future.
 */
void MultiGexpro::findCommonFeatures() {
  // No suitable STL merge algorithm for variable number of vectors

  // std::vector< std::vector<std::string> > sorted_feature_vecs(gps.size());
  // std::vector<std::string> temp_feature_vec;
  // for (auto i: gps) {
  //   temp_feature_vec = *i;  // dereferencing makes a copy
  //   std::sort(temp_feature_vec.begin(), temp_feature_vec.end());
  //   sorted_feature_vecs.push_back(temp_feature_vec);
  // }

  std::vector<std::string> cfs;

  std::vector<std::string> merged_feature_vecs;
  std::vector<std::string>* fidx;
  for (auto i: gps) {
    fidx = i->getFeatureNames();
    std::copy(fidx->begin(), fidx->end(), std::back_inserter(merged_feature_vecs));
  }

  std::string previous = "";
  for (auto j: merged_feature_vecs) {
    if (j.compare(previous) == 0)
      continue;
    cfs.push_back(j);
    previous = j;
  }

  common_features = cfs;
}
