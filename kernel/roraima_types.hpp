#ifndef RORAIMA_TYPES_HPP
#define RORAIMA_TYPES_HPP

#include <limits>
#include <vector>
#include <queue>

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
  std::vector<double> item;
  double lambda;
  double bm_indx;
  double norm;
  int k;
};

struct min_heap_cmp {
  inline bool operator() (const std::pair<std::size_t, double >& l,
  		const std::pair<std::size_t, double >& r) {
    return l.second > r.second;		
  }
};

struct heap_node {
  heap_node(std::size_t id, double v) {
    val = std::pair<std::size_t, double>(id, v);
  }
  std::pair<std::size_t, double> val;
};

using min_heap = std::priority_queue<std::pair<std::size_t, double>, std::vector<std::pair<std::size_t, double> >, min_heap_cmp>;

} // namespace roraima

#endif
