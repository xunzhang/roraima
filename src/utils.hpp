#ifndef UTILS_HPP
#define UTILS_HPP

#include <numeric>
#include "roraima_types.hpp"

namespace roraima {

inline double dot_product(const vector<double> & a, const vector<double> & b) {
  assert(a.size() == b.size());
  return std::inner_product(a.begin(), a.end(), b.begin(), 0.);
}

inline double norm(const vector<double> & a) {
  double norm = 0.;
  for(auto & i : a) { norm += i * i; }
  return sqrt(norm);
}

inline double eculid_dist(const vector<double> & a, const vector<double> & b) {
  double sum = 0.; 
  for(int i = 0; i < a.size(); ++i) {
    double s = b[i] - a[i];
    sum += s * s;
  }
  return sqrt(sum);
}		

}  // namespace roraima

#endif
