#ifndef CACHE_HPP
#define CACHE_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace roraima {

template <class K, class V>
struct Node {
  K key;
  V val;
  Node *l, *r;
};

template <class K, class V>
class lru_cache {
public:
  lru_cache() {
    head = new Node<K, V>;
    tail = new Node<K, V>;
    _init();
  }
  lru_cache(size_t n) : sz(n) {
    head = new Node<K, V>;
    tail = new Node<K, V>;
    _init();
  }
  
  lru_cache(const lru_cache& cache) : sz(cache.sz) {
    for(auto it = tail; it != head; it = it->l)
      Put(it->key, it->val);
  }
  
  lru_cache& operator=(const lru_cache &cache) {
    sz = cache.sz;
    for(auto it = tail; it != head; it = it->l)
      Put(it->key, it->val);
  }
  
  ~lru_cache() {
    delete head;
    delete tail;
  }
   
  V Get(K key) {
    auto it = hashmap.find(key);
    if(it == hashmap.end()) {
      // not found  
      return V(); 
    } else {
      detach(it->second);
      attach(it->second);
      return it->second->val;
    }
  }

  void Put(K key, V val) {
    Node<K, V>* new_node = new Node<K, V>;
    auto it = hashmap.find(key);
    if(it == hashmap.end()) {
      // not in cache yet
      if(sz != 0) {
        // cache is not full
        attach(new_node);
        sz -= 1;
      } else {
        // cache is full
        hashmap.erase(tail->l->key);
        detach(tail->l);
        attach(new_node);
      }
      new_node->key = key;
      new_node->val = val;
      hashmap[key] = new_node;
    } else {
      // already in cache
      detach(it->second);
      it->second->val = val;
      attach(it->second);
    }
  }
  
  void clear() {
    /* delete dll */
    auto it = head->r;
    while(it != tail) {
      auto tmp = it;
      it = it->r;
      delete tmp;
    }
    _init();
    /* delete hashmap */
    sz = hashmap.size();
    hashmap.clear();
  }
  
  void traverse_print() {
    std::cout << "size is " << sz << std::endl;
    for(auto it = head; it != tail; it = it->r) 
      std::cout << it->val << " ";
    std::cout << std::endl;
  }

private:
  void _init() {
    head->l = NULL;
    head->r = tail;
    tail->l = head;
    tail->r = NULL;
  }
  // attach node @ head
  void attach(Node<K, V>* node) {
    node->l = head;
    node->r = head->r;
    head->r = node;
    node->r->l = node;
  }
  
  void detach(Node<K, V>* node) {
    node->l->r = node->r;
    node->r->l = node->l;
  }

private:
  size_t sz = 3;
  Node<K, V> *head, *tail;
  std::unordered_map<K, Node<K, V>* > hashmap;
};

} // namespace roraima

#endif
