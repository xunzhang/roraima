#ifndef BALLTREE_HPP
#define BALLTREE_HPP

#include <math.h>
#include <vector>

using std::vector;

namespace roraima {

struct balltree_node {
  
  balltree_node(vector<size_t> input_indices) : radius(0.), left(0), right(0) { 
    indices = input_indices; 
  }

  double radius;
  balltree_node* left;
  balltree_node* right;
  vector<double> miu;
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
  
  double ecuild_dist(const vector<double> & a, const vector<double> & b) {
    double sum = 0.;
    for(int i = 0; i < a.size(); ++i) {
      double s = b[i] - a[i];
      sum += s * s;
    }
    return sqrt(sum);
  }

  /* calculate center of gravity */
  std::vector<double> cal_mean(const vector<size_t> & ids) {
    std::vector<double> center(ids.size());
    for(auto & id : ids) {
      int i = 0;
      for(auto & dim : items[id]) {
        center[i] += dim;
	i += 1;
      }
    }
    double tmp = 1. / ids.size();
    for(int i = 0; i < center.size(); ++i) {
      center[i] *= tmp;
    }
    return center;
  }
  
  /* calculate radius: farthest from center */
  double cal_maxr(const vector<size_t> & ids,
  		const vector<double> & miu) {
    vector<double> dist_lst;
    for(auto & id : ids) {
      dist_lst.emplace_back(eculid_dist(items[id], miu));
    }
    return std::max_element(dist_lst);
  }

  void split_indices(vector<size_t> indices, vector<size_t> & lc_indices, vector<size_t> & rc_indices) {
    
  }

  balltree_node* build_recsive(vector<size_t> & indices) {
    
    balltree_node* node = new balltree_node(indices);
    node->miu = cal_mean(indices);
    node->radius = cal_maxr(indices, node->miu);

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
