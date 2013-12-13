#ifndef BALLTREE_HPP
#define BALLTREE_HPP

#include <iostream>
#include <assert.h>
#include <stdio.h>  /* NULL */
#include <stdlib.h>  /* rand */
#include <time.h>  /* time */
#include <cstddef>
#include <math.h>
#include <vector>
#include <algorithm>

using std::vector;

#include "roraima_types.hpp"
#include "utils.hpp"

namespace roraima {

struct balltree_node {
  
  balltree_node(vector<std::size_t> input_indices) : radius(0.), left(0), right(0) { 
    indices = input_indices; 
  }
  
  ~balltree_node() {
    delete left;
    delete right;
    indices.resize(0);
  }

  double radius;
  balltree_node* left;
  balltree_node* right;
  vector<double> miu;
  vector<std::size_t> indices;
};

struct balltree {

public:

  balltree(std::vector<std::vector<double> > items) : root(0), limit(4) { this->items = items; }

  balltree(int limit, std::vector<std::vector<double> > items) : root(0) { 
    this->limit = limit;
    this->items = items;   
  }
  
  ~balltree() {
    delete root;
    items.resize(0);
  }

  void build() {
    vector<std::size_t> indices(items.size());
    for(std::size_t i = 0; i < items.size(); ++i) { indices[i] = i; }
    root = build_recsive(indices);
  }
  
  void insert(const vector<double> & item) {}
  
  void insert(const balltree_node  & node) {}

  void insert(const vector<vector<double> > & items) {
    for(auto & item : items) {
      insert(item);
    }
  }
  
  void remove(const vector<double> & item) {}

  void remove(const balltree_node & node) {}

  void remove(const vector<vector<double> > & items) {
    for(auto & item : items) {
      remove(item);
    }
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
      /*
      std::cout << "srt" << std::endl;
      for(auto & i : lc_indices)
        std::cout << i << std::endl;
      std::cout << "mid" << std::endl;
      for(auto & i : rc_indices)
        std::cout << i << std::endl;
      std::cout << "end" << std::endl;
      */
      node->left = build_recsive(lc_indices);
      node->right = build_recsive(rc_indices);
      return node;
    } 
  }

public:
  int limit;
  balltree_node *root;
  vector<vector<double> > items;
};

double max_inner_product(const roraima::query & q,
			const vector<double> & miu,
			double radius) {
  return roraima::dot_product(q.item, miu) + q.norm * radius;
}

std::size_t linear_search(const vector<std::size_t> & ids,
		const balltree & stree,
		roraima::query & q) {
  std::size_t res_id = 0;
  for(auto & id : ids) {
    auto pdt = roraima::dot_product(stree.items[id], q.item);
    if(pdt > q.lambda) {
      q.lambda = pdt;
      res_id = id;
    }
  }
  return res_id;
}

void balltree_search(const balltree & stree,
		balltree_node *node,
		const roraima::query & q,
		vector<std::size_t> & result) {
		/*
    if(q.lambda < roraima::max_inner_product(q.item, node.miu, node.radius)) {
      // this node has potential
      if() {
      
      } else {
        // best depth first traversal
	auto v_left = roraima::max_inner_product(q->item, node->left->miu, node->left->radius);
	auto v_right = roraima::max_inner_product(q->item, node->right->miu, node->right->radius);
	if(v_left <= v_right) {
	  balltree_search(stree, );
	  balltree_search();
	} else {
	  balltree_search();
	  balltree_search();
	}
      }
    } else { 
      // Else the node is pruned from computation
    }
    */
}

// search api
void search(const roraima::query & q, std::size_t k,
	const balltree & stree,
	vector<std::size_t> & result) {
  result.resize(0);
  balltree_search(stree, stree.root, q, result);
}

} // namespace roraima

#endif
