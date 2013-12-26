#ifndef RORAIMA_TYPES_HPP
#define RORAIMA_TYPES_HPP

#include <limits>
#include <vector>
#include <queue>
#include <unordered_map>

#include "utils.hpp"

namespace roraima {

struct query {
  query(std::vector<double> item) : lambda(std::numeric_limits<double>::lowest()), bm_indx(0), k(0) { 
    this->item = item; 
    this->norm = roraima::norm(item); 
  }
  query(std::vector<double> item, int k) : lambda(std::numeric_limits<double>::lowest()), bm_indx(0) { 
    this->k = k;
    this->item = item; 
    this->norm = roraima::norm(item);  
  }
  query(std::unordered_map<long, char> blacklst, std::vector<double> item, int k) : lambda(std::numeric_limits<double>::lowest()), bm_indx(0) { 
    this->k = k;
    this->item = item;
    this->blacklst = blacklst;
    this->norm = roraima::norm(item);  
  }
  std::vector<double> item;
  std::unordered_map<long, char> blacklst;
  double lambda;
  double bm_indx;
  double norm;
  int k;
};

struct min_heap_cmp {
  inline bool operator() (const std::pair<long, double >& l,
  		const std::pair<long, double >& r) {
    return l.second > r.second;		
  }
};

struct heap_node {
  heap_node(long id, double v) {
    val = std::pair<long, double>(id, v);
  }
  std::pair<long, double> val;
};

using min_heap = std::priority_queue<std::pair<long, double>, std::vector<std::pair<long, double> >, min_heap_cmp>;

} // namespace roraima

#endif
