#include <vector>
#include <iostream>
#include <cstddef>
#include "balltree.hpp"
#include "search.hpp"
int main(int argc, char *argv[]) {
  std::vector<std::vector<double> > items = {{2.8, 3.9}, {2.1, 2.7}, {2.8, 3.1}, {3., 2.8},
  					{3.1, 3.}, {2.6, 9.1}, {3.5, 9.2}, {3.1, 8.6},
					{3.6, 8.8}, {8.2, 7.6}, {9.2, 8.5}, {9.3, 7.5}, 
					{8.3, 6.3}, {8., 6.}, {8.4, 6.1}, {9., 6.4},
					{9.4, 6.8}, {9.2, 6.6}, {9.1, 6.1}, {7.9, 3.7},
					{8.8, 3.2}, {9.1, 2.7}, {8.7, 1.8}, {8.9, 1.5}};
  std::vector<long> r;
  roraima::balltree<double, roraima::eculid_dist> stree(items);
  std::vector<long> indices = {23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  stree.build(indices);
  //stree.build();
  std::cout << "built" << std::endl;
  std::vector<double> tmp = {1., 2.};
  roraima::query q(tmp, 10);
  roraima::search(q, stree, r);
  for(auto & i : r)
    std::cout << i << std::endl;
  return 0;
}
