#ifndef RORAIMA_TYPES_HPP
#define RORAIMA_TYPES_HPP

#include <cfloat>
#include "utils.hpp"

namespace roraima {

struct query {
  query(vector<double> item) : lambda(DBL_MIN), bm_indx(0) { 
    this->item = item; 
    this->norm = roraima::norm(item);  
  }
  vector<double> item;
  double lambda;
  double bm_indx;
  double norm;
};

} // namespace roraima

#endif
