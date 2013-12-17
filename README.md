Motivation
==========
Online retrieval of recommendation, based on Factor Model.
Give a user-id, roraima will return top item-ids which will get top ratings for further usage.

Challenge
---------
Factor Model is a very useful/good model for recommendation system and a lot of research is there. Inner-product is the kernel operation used for predict rating in this model.
There are some great ideas and work we could take. Firstly, we can build a tree(kdtree, balltree, vptree...) to do exact efficient retrieval. Approximately, we can use LSH(Locality Sensitive Hashing) framework to do even faster retrieval( O(const) ).
But problems here is totally different because inner-product-measure do not follow 'the three rules' in measure space which seperate it from eculid distance, cosine distance, jaccard, etc...

Roraima Solution
----------------
I. cosine factor model + lsh

II. dimension reduction + tree search

Installation
------------
Modify 'Makefile', then ```$make```
Notice: C++11(g++ version >= 4.7 or clang) | gflags(or douban's option-parse-tool) must be supported


Reference
---------
P. Ram & A. Gray, Maximum Inner-Product Search using Tree Data-Structures, 2012.
[More detal](http://www.cc.gatech.edu/~pram/projs/IPSS.html)
