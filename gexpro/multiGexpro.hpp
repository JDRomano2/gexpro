#ifndef MULTIGEXPRO_HPP
#define MULTIGEXPRO_HPP

/*
 * Class that holds one or more instances of gexpro.
 *
 * Loosely follows the "Composite" structural pattern from Design Patterns book
 * (but not exactly - Gexpro is not a descendant of MultiGexpro).
 */

#include <vector>
#include <set>
#include <algorithm>
#include <functional>

#include "gexpro.hpp"

class MultiGexpro {
  std::vector<Gexpro> gps;
  std::vector<std::string> common_features;   // 'features' are transcripts, probes, etc.
  std::vector<std::string> uncommon_features; // features that aren't in every gexpro
public:
  MultiGexpro() : gps() {};
  MultiGexpro(Gexpro& firstGexpro);

  const Gexpro* getGpByIndex(int gp_index);
  const Gexpro* getGpByName(std::string gp_name);
  void add(Gexpro& gp);
  //void remove(Gexpro& gp);
  void dumpAllMatrices();

  // data manipulations
  void findCommonFeatures();
  void removeNonCommonFeatures();  // Must have run findCommonFeatures()
  void imputeZeros();  // Impute non-common features as zeros

  // statistical manipulations
  void normalizeAllGexpros();

  // informational methods
  size_t numGexpros() const { return gps.size(); }
  size_t numCommonFeatures() const { return common_features.size(); }
};

#endif
