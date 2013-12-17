#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <assert.h>
#include <vector>
#include "balltree.hpp"
#include "roraima_types.hpp"
#include "utils.hpp"

using std::vector;

//#define DIST roraima::eculid_dist
constexpr auto DIST = roraima::eculid_dist;

namespace roraima {

double max_inner_product(const roraima::query & q,
			const vector<double> & miu,
			double radius) {
  return roraima::dot_product(q.item, miu) + q.norm * radius;
}

void linear_search_array(const vector<std::size_t> & ids,
		const roraima::balltree<double, DIST> & stree,
		roraima::query & q,
		vector<std::pair<std::size_t, double> > & tmplst) {
  for(auto & id : ids) {
    auto pdt = roraima::dot_product(stree.items[id], q.item);
    tmplst.push_back(std::pair<std::size_t, double>(id, pdt));
  }
  std::sort(tmplst.begin(), tmplst.end(), 
  	[] (std::pair<std::size_t, double> a, 
	std::pair<std::size_t, double> b) {
  	  return a.second > b.second;
	});
  if((int)tmplst.size() >= q.k) {
    q.lambda = tmplst[tmplst.size() - 1].second;
  }
}

void linear_search_heap(const vector<std::size_t> & ids,
		const roraima::balltree<double, DIST> & stree,
		roraima::query & q,
		roraima::min_heap & tmplst) {
  for(auto & id : ids) {
    auto pdt = roraima::dot_product(stree.items[id], q.item);
    auto node = roraima::heap_node(id, pdt);
    tmplst.push(node.val);
    if((int)tmplst.size() > q.k) tmplst.pop();
  }
  assert((int)tmplst.size() <= q.k);
  if((int)tmplst.size() == q.k) {
    q.lambda = tmplst.top().second;
  }
}

/* array impl */
void balltree_search(const roraima::balltree<double, DIST> & stree,
		balltree_node *node,
		roraima::query & q,
		vector<std::pair<std::size_t, double> > & tmplst) {
    auto v = roraima::max_inner_product(q.item, node->miu, node->radius);
    if(q.lambda < v) {
      // this node has potential
      if((!node->left) && (!node->right)) {
        linear_search_array(node->indices, stree, q, tmplst);
      } else {
        // best depth first traversal
	auto v_left = roraima::max_inner_product(q.item, node->left->miu, node->left->radius);
	auto v_right = roraima::max_inner_product(q.item, node->right->miu, node->right->radius);
	if((q.lambda < v_left) || (q.lambda < v_right)) {
	  if(v_left < v_right) {
	    balltree_search(stree, node->right, q, tmplst);
	    if(q.lambda < v_left)
	      balltree_search(stree, node->left, q, tmplst);
	  } else {
	    balltree_search(stree, node->left, q, tmplst);
	    if(q.lambda < v_right)
	      balltree_search(stree, node->right, q, tmplst);
	  }
	}
      }
    } else { 
      // Else the node is pruned from computation
    }
}

/* heap impl */
void balltree_search(const roraima::balltree<double, DIST> & stree,
		balltree_node *node,
		roraima::query & q,
		roraima::min_heap & tmplst) {
    auto v = roraima::max_inner_product(q.item, node->miu, node->radius);
    if(q.lambda < v) {
      // this node has potential
      if((!node->left) && (!node->right)) {
        linear_search_heap(node->indices, stree, q, tmplst);
      } else {
        // best depth first traversal
	auto v_left = roraima::max_inner_product(q.item, node->left->miu, node->left->radius);
	auto v_right = roraima::max_inner_product(q.item, node->right->miu, node->right->radius);
	if((q.lambda < v_left) || (q.lambda < v_right)) {
	  if(v_left < v_right) {
	    balltree_search(stree, node->right, q, tmplst);
	    if(q.lambda < v_left)
	      balltree_search(stree, node->left, q, tmplst);
	  } else {
	    balltree_search(stree, node->left, q, tmplst);
	    if(q.lambda < v_right)
	      balltree_search(stree, node->right, q, tmplst);
	  }
	}
      }
    } else { 
      // Else the node is pruned from computation
    }
}

// search api
void search(roraima::query & q,
	const roraima::balltree<double, DIST> & stree,
	vector<std::size_t> & result) {
  result.resize(0);
  //vector<std::pair<std::size_t, double> > tmplst;
  roraima::min_heap tmplst;
  balltree_search(stree, stree.root, q, tmplst);
  assert(q.k <= (int)tmplst.size());
  /*
  for(int i = 0; i < q.k; ++i) {
    result.push_back(tmplst[i].first);
  }
  */
  while(!tmplst.empty()) {
    result.push_back(tmplst.top().first);
    tmplst.pop();
  }
  std::reverse(result.begin(), result.end());
}

} // namespace roraima

#endif
