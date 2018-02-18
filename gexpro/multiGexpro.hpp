#ifndef MULTIGEXPRO_HPP
#define MULTIGEXPRO_HPP

/*
 * Class that holds one or more instances of gexpro.
 *
 * Loosely follows the "Composite" structural pattern from Design Patterns book
 * (but not exactly - Gexpro is not a descendant of MultiGexpro).
 */

#include <vector>
#include <algorithm>

#include "gexpro.hpp"

class MultiGexpro {
  std::vector<const Gexpro*> gps;
  std::vector<std::string> common_features;  // 'features' are transcripts, probes, etc.
public:
  MultiGexpro(const Gexpro& firstGexpro);

  const Gexpro* getGpByIndex(int gp_index);
  const Gexpro* getGpByName(std::string gp_name);
  void add(Gexpro& gp);
  void remove(Gexpro& gp);

  // data manipulations
  void findCommonFeatures();

  // statistical manipulations


  // informational methods
  size_t numGexpros() { return gps.size(); }
};

#endif
