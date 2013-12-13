#ifndef BALLTREE_HPP
#define BALLTREE_HPP

#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* rand */
#include <time.h>       /* time */
#include <cstddef>
#include <math.h>
#include <vector>
#include <algorithm>

using std::vector;

namespace roraima {

struct balltree_node {
  
  balltree_node(vector<std::size_t> input_indices) : radius(0.), left(0), right(0) { 
    indices = input_indices; 
  }

  double radius;
  balltree_node* left;
  balltree_node* right;
  vector<double> miu;
  vector<std::size_t> indices;
};


struct balltree {
public:

  balltree(std::vector<std::vector<double> > items) : root(0), limit(30) { this->items = items; }

  balltree(int limit, std::vector<std::vector<double> > items) : root(0) { 
    this->limit = limit;
    this->items = items;   
  }
  
  void build() {
    vector<std::size_t> indices(items.size());
    for(std::size_t i = 0; i < items.size(); ++i) { indices[i] = i; }
    root = build_recsive(indices);
  }
  
private:
  
  double eculid_dist(const vector<double> & a, const vector<double> & b) {
    double sum = 0.;
    for(int i = 0; i < a.size(); ++i) {
      double s = b[i] - a[i];
      sum += s * s;
    }
    return sqrt(sum);
  }

  /* calculate center of gravity */
  std::vector<double> cal_mean(const vector<std::size_t> & ids) {
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
  double cal_maxr(const vector<std::size_t> & ids,
  		const vector<double> & miu) {
    vector<double> dist_lst;
    for(auto & id : ids) {
      dist_lst.emplace_back(eculid_dist(items[id], miu));
    }
    return *std::max_element(dist_lst.begin(), dist_lst.end());
  }
  
  vector<double> arg_max(const vector<double> & basis,
  			const vector<std::size_t> & cmp_ids) {
    std::size_t maxid;
    double t = 0.;
    for(auto & id : cmp_ids) {
      double d = eculid_dist(basis, items[id]);
      auto tmp = d * d;
      if(tmp > t) {
        t = tmp;
        maxid = id;
      }
    }
    return items[maxid];
  }
  
  void partition(const vector<std::size_t> & ids, 
  		const vector<double> & xa,
		const vector<double> & xb,
		vector<std::size_t> & lc_ids,
		vector<std::size_t> & rc_ids) {		
    lc_ids.resize(0); rc_ids.resize(0);
    for(auto & id : ids) {
      auto d1 = eculid_dist(items[id], xa);
      auto d2 = eculid_dist(items[id], xb);
      if(d1 <= d2) {
        lc_ids.push_back(id);
      } else {
        rc_ids.push_back(id);
      }
    }  
  }

  void split_indices(vector<std::size_t> ids, 
  		vector<std::size_t> & lc_ids, 
		vector<std::size_t> & rc_ids) { 
    srand(time(NULL));
    auto id = rand() % ids.size();
    auto xa = arg_max(items[id], ids);
    auto xb = arg_max(xa, ids);
    partition(ids, xa, xb, lc_ids, rc_ids);
  }

  balltree_node* build_recsive(vector<std::size_t> & indices) {
    
    balltree_node* node = new balltree_node(indices);
    node->miu = cal_mean(indices);
    node->radius = cal_maxr(indices, node->miu);

    if(indices.size() < limit) {
      return node;
    } else {
      vector<std::size_t> lc_indices, rc_indices;
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
	std::size_t k,
	vector<std::size_t> *result) {}

} // namespace roraima

#endif
