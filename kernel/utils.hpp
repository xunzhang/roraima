#ifndef UTILS_HPP
#define UTILS_HPP

#include <assert.h>
#include <math.h>
#include <numeric>
#include <string>
#include <vector>

using std::vector;

namespace roraima {

bool startswith(const std::string & str, const std::string & key) {
  return str.find(key) == 0;
}

vector<std::string> str_split(const std::string & str, char sep) {
  vector<std::string> result;
  size_t st = 0, en = 0;
  while(1) {
    en = str.find(sep, st);
    auto s = str.substr(st, en - st);
    if(s != "") result.push_back(s);
    if(en == std::string::npos) break;
    st = en + 1;
  }
  return result;
}

vector<std::string> str_split(const std::string & str, const std::string & seps) {
  vector<std::string> result;
  size_t st = 0, en = 0;
  while(1) {
    en = str.find_first_of(seps, st);
    auto s = str.substr(st, en - st);
    if(s != "") result.push_back(s);
    if(en == std::string::npos) break;
    st = en + 1;
  }
  return result;
}

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
  for(int i = 0; i < (int)a.size(); ++i) {
    double s = b[i] - a[i];
    sum += s * s;
  }
  return sqrt(sum);
}		

}  // namespace roraima

#endif
