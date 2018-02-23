#include "multiGexpro.hpp"

// Debugging - see below
#include <type_traits>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>
#include <cstdlib>

// // For debugging (see https://stackoverflow.com/a/20170989/1730417)
// template <class T>
// std::string
// type_name()
// {
//   typedef typename std::remove_reference<T>::type TR;
//   std::unique_ptr<char, void(*)(void*)> own
//     (
// #ifndef _MSC_VER
//      abi::__cxa_demangle(typeid(TR).name(), nullptr,
//                          nullptr, nullptr),
// #else
//      nullptr,
// #endif
//      std::free
//      );
//   std::string r = own != nullptr ? own.get() : typeid(TR).name();
//   if (std::is_const<TR>::value)
//     r += " const";
//   if (std::is_volatile<TR>::value)
//     r += " volatile";
//   if (std::is_lvalue_reference<T>::value)
//     r += "&";
//   else if (std::is_rvalue_reference<T>::value)
//     r += "&&";
//   return r;
// }

MultiGexpro::MultiGexpro(Gexpro& firstGexpro) {
  // Unfortunately we can't make a vector of references; store instead as vector of pointers
  // (perhaps reimplement as boost::ptr_vector)
  Gexpro *gp = &firstGexpro;
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
  // THIS CAN BE MADE MORE EFFICIENT!

  std::vector<std::string> cfs;

  std::vector<std::string> merged_feature_vecs;
  std::vector<std::string>* fidx;
  for (auto i: gps) {
    std::vector<std::string> temp = i->getFeatureNames();
    fidx = &temp;
    std::copy(fidx->begin(), fidx->end(), std::back_inserter(merged_feature_vecs));
  }

  // Perform lexicographic sort
  std::sort(merged_feature_vecs.begin(), merged_feature_vecs.end());

  std::string previous = "";
  for (auto j: merged_feature_vecs) {
    if (j.compare(previous) == 0)
      continue;
    cfs.push_back(j);
    previous = j;
  }

  std::map<std::string, int> feat_map;
  for (auto const& x : cfs) { feat_map[x] = 0; }

  // Iterate over all features. Increment feat_map for every value
  int num_obs_gexpros = 0;
  for (auto k: gps) {
    num_obs_gexpros++;
    for (auto l: k->getFeatureNames()) {
      (feat_map[l])++;
    }
  }

  std::vector<std::string> in_all;
  std::vector<std::string> not_in_all;

  std::map<std::string, int>::iterator m_it = feat_map.begin();
  while (m_it != feat_map.end()) {
    if (m_it->second == num_obs_gexpros) {
      in_all.push_back(m_it->first);
    } else {
      not_in_all.push_back(m_it->first);
    }
    m_it++;
  }

  common_features = in_all;
  uncommon_features = not_in_all;
}

void MultiGexpro::removeNonCommonFeatures() {
  // replace the following with (VERBOSE)
  // if (true) {
  //   std::cout << "==============================================" << std::endl;
  //   std::cout << "REMOVING NON-COMMON FEATURES" << std::endl;
  //   std::cout << std::endl;
  //   std::cout << "Number of common features: " << common_features.size() << std::endl;
  //   std::cout << "  Number of features in gp1: " << gps[0]->getNFeatures() << std::endl;
  //   std::cout << "  Number of samples in gp1: " << gps[0]->getNSamples() << std::endl;
  //   std::cout << std::endl;
  //   std::cout << "  Number of features in gp2: " << gps[1]->getNFeatures() << std::endl;
  //   std::cout << "  Number of samples in gp2: " << gps[1]->getNSamples() << std::endl;
  //   std::cout << std::endl;
  // }

  for (auto i: uncommon_features)
    for (auto j: gps)
      j->removeFeatureByName(i, true);
}

void MultiGexpro::imputeZeros() {

}

void MultiGexpro::normalizeAllGexpros() {
  std::cout << "Normalizing all data..." << std::endl;
  for (auto i: gps) {
    i->normalizeFromDataMatrix();
  }
  std::cout << "...successfully normalized all datasets." << std::endl;
}
