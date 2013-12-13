#include <vector>
#include <iostream>
#include "balltree.hpp"

int main(int argc, char *argv[]) {
  std::vector<std::vector<double> > items = {{2, 3}, {2, 2},
  					{4, 4}, {3, 4}, {1, 2},
					{3, 4}, {3, 2}, {1, 2}, 
					{3, 3}, {3, 3}};
  roraima::balltree stree(items);
  stree.build();
  return 0;
}
