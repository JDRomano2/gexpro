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
  std::cout << "Initialized data structures in findCommonFeatures()" << std::endl;
  for (auto i: gps) {
    std::vector<std::string> temp = i->getFeatureNames();
    fidx = &temp;
    std::copy(fidx->begin(), fidx->end(), std::back_inserter(merged_feature_vecs));
  }

  // Perform lexicographic sort
  std::cout << "Sorting aggregate list of features..." << std::endl;
  std::sort(merged_feature_vecs.begin(), merged_feature_vecs.end());

  std::cout << "Built aggregate list of features; now merging..." << std::endl;

  std::string previous = "";
  for (auto j: merged_feature_vecs) {
    if (j.compare(previous) == 0)
      continue;
    cfs.push_back(j);
    previous = j;
  }

  common_features = cfs;
  std::cout << "NUM COMMON FEATURES: " << cfs.size() << std::endl;
}

void MultiGexpro::normalizeAllGexpros() {
  std::cout << "Normalizing all data..." << std::endl;
  for (auto i: gps) {
    i->normalizeFromDataMatrix();
  }
  std::cout << "...successfully normalized all datasets." << std::endl;
}
