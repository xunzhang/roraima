#ifndef UTILS_HPP
#define UTILS_HPP

#include "roraima_types.hpp"

namespace roraima {

double dot_product(const vector<double> & a, const vector<double> & b) {
  assert(a.size() == b.size());
  double pdt = 0.;
  for(int i = 0; i < a.size(); ++i) {
    pdt += a[i] * b[i];
  }
  return pdt;
}

double norm(const vector<double> & a) {
  double norm = 0.;
  for(auto & i : a) { norm += i * i; }
  return sqrt(norm);
}

}  // namespace roraima

#endif
