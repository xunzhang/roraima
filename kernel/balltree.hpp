#ifndef BALLTREE_HPP
#define BALLTREE_HPP

#include <iostream>
#include <assert.h>
#include <stdio.h>  /* NULL */
#include <stdlib.h>  /* rand */
#include <time.h>  /* time */
#include <vector>
#include <algorithm>

using std::vector;

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

template <class T,
	double (*dist)(const vector<T> & , 
		const vector<T> &)>
struct balltree {

public:

  balltree(vector<vector<T> > items) : limit(20), root(0) { this->items = items; }

  balltree(int limit, vector<vector<T> > items) : root(0) { 
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
  
  // TODO
  void insert(const vector<T> & item) {}
  
  // TODO
  void insert(const balltree_node  & node) {}

  void insert(const vector<vector<T> > & items) {
    for(auto & item : items) {
      insert(item);
    }
  }
  
  // TODO
  void remove(const vector<T> & item) {}

  // TODO
  void remove(const balltree_node & node) {}

  void remove(const vector<vector<T> > & items) {
    for(auto & item : items) {
      remove(item);
    }
  }

private:

  /* calculate center of gravity */
  vector<T> cal_mean(const vector<std::size_t> & ids) {
    vector<T> center(items[0].size());
    for(auto & id : ids) {
      int i = 0;
      for(auto & dim : items[id]) {
        center[i] += dim;
	i += 1;
      }
    }
    T tmp = 1. / ids.size();
    for(int i = 0; i < (int)center.size(); ++i) {
      center[i] *= tmp;
    }
    return center;
  }
  
  /* calculate radius: farthest from center */
  double cal_maxr(const vector<std::size_t> & ids,
  		const vector<double> & miu) {
    vector<double> dist_lst;
    for(auto & id : ids) {
      dist_lst.emplace_back(dist(items[id], miu));
    }
    return *std::max_element(dist_lst.begin(), dist_lst.end());
  }
  
  vector<T> arg_max(const vector<T> & basis,
  			const vector<std::size_t> & cmp_ids) {
    std::size_t maxid;
    double t = 0.;
    for(auto & id : cmp_ids) {
      double d = dist(basis, items[id]);
      auto tmp = d * d;
      if(tmp > t) {
        t = tmp;
        maxid = id;
      }
    }
    return items[maxid];
  }
  
  void partition(const vector<std::size_t> & ids, 
  		const vector<T> & xa,
		const vector<T> & xb,
		vector<std::size_t> & lc_ids,
		vector<std::size_t> & rc_ids) {		
    lc_ids.resize(0); rc_ids.resize(0);
    for(auto & id : ids) {
      auto d1 = dist(items[id], xa);
      auto d2 = dist(items[id], xb);
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

    if((int)indices.size() <= limit) {
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
  vector<vector<T> > items;
};

} // namespace roraima

#endif
