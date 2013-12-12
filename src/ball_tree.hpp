#ifndef BALLTREE_HPP
#define BALLTREE_HPP

#include <vector>
using std::vector;

namespace roraima {

struct balltree_node {
  
  balltree_node(vector<size_t> input_indices) : miu(0.), radius(0.), left(0), right(0) { 
    indices = input_indices; 
  }

  double miu;
  double radius;
  balltree_node* left;
  balltree_node* right;
  vector<size_t> indices;
};


struct balltree {
public:

  balltree(std::vector<std::vector<double> > items) : root(0), limit(30) { this->items = items; }

  balltree(int limit, std::vector<std::vector<double> > items) : root(0) { 
    this->limit = limit;
    this->items = items;   
  }
  
  void build() {
    vector<size_t> indices(items.size());
    for(size_t i = 0; i < items.size(); ++i) { indices[i] = i; }
    root = build_recsive(indices);
  }

private:
  
  void split_indices(vector<size_t> indices, vector<size_t> & lc_indices, vector<size_t> & rc_indices) {}

  balltree_node* build_recsive(vector<size_t> & indices) {
    balltree_node* node = new balltree_node(indices);
    node->miu = 0.;
    node->radius = 0.;
    if(indices.size() < limit) {
      return node;
    } else {
      vector<size_t> lc_indices, rc_indices;
      split_indices(node->indices, lc_indices, rc_indices);
      node->left = build_recsive(lc_indices);
      node->right = build_recsive(rc_indices);
      return node;
    } 
  }

private:
  int limit;
  balltree_node *root;
  vector<vector<double> > items;
};

void search(const balltree & stree, 
	const vector<double> & query_item, 
	size_t k,
	vector<size_t> *result) {}

} // namespace roraima

#endif
