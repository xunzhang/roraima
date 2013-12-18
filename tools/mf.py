#! /usr/bin/python
# Basic Matrix factorization 

import numpy as np

class bmf():
    def __init__(self, k = 100, rounds = 50, alpha = 0.005, beta = 0.02, train_fn = '', validate_fn = '', pred_fn = '', output = ''):
        self.k = k
	self.rounds = rounds
	self.alpha = alpha
	self.beta = beta
	self.train_fn = train_fn
	self.pred_fn = pred_fn
	self.validate_fn = validate_fn
	self.output = output
        self.usr_dct = {}
	self.item_dct = {}
	self.reverse_usr_dct = {}
	self.reverse_item_dct = {}
	self.rating_graph = {}
	self.rating_sz = 0
	self.p = None
	self.q = None
	self.rmse = 0.
    
    def load(self):
        f = open(self.train_fn)
	for line in f:
	    uid, iid, rating = line.strip('\n').split(',')
            rating = float(rating)
	    if uid not in self.usr_dct:
	        self.usr_dct[uid] = len(self.usr_dct)
	    if iid not in self.item_dct:
	        self.item_dct[iid] = len(self.item_dct)
	    self.rating_graph.setdefault(self.usr_dct[uid], []).append((self.item_dct[iid], rating))
	    self.rating_sz += 1
	# init reverse map(for write usage)
	self.reverse_usr_dct = {v:k for k, v in self.usr_dct.items()}
	self.reverse_item_dct = {v:k for k, v in self.item_dct.items()}
	f.close()
    
    def cal_rmse(self):
        import math
        for u_indx, pair in self.rating_graph.iteritems():
	    for i_indx, rating in pair:
	        tmp = np.dot(self.p[u_indx, :], self.q[i_indx, :])
	        self.rmse += (rating - tmp) ** 2
        return math.sqrt(self.rmse / self.rating_sz)

    def predict_rating(self):
        f1 = open(self.pred_fn)
	f2 = open(self.output, 'w')
	for line in f1:
	    uid, iid = line.strip('\n').split(',')
	    u_indx = self.usr_dct[uid]
	    i_indx = self.item_dct[iid]
	    pred_rating = np.dot(self.p[u_indx, :], self.q[i_indx, :])
	    f2.write('%s,%s,%s\n' % (uid, iid, pred_rating))
	f1.close()
	f2.close()

    def r(self, i, j):
        return np.dot(self.p[i, :], self.q[j, :])

    def kernel(self):
        import time
	# init parameters
	self.p = np.random.rand(len(self.usr_dct), self.k)
        self.q = np.random.rand(len(self.item_dct), self.k)
	# learning
	for rd in xrange(self.rounds):
	    start = time.time()
	    for u_indx, pair in self.rating_graph.iteritems():
	        for i_indx, rating in pair:
		    tmp = np.dot(self.p[u_indx, :], self.q[i_indx, :])
		    e = rating - tmp 
		    # learning delta
		    delta_p = self.alpha * (2 * e * self.q[i_indx, :] - self.beta * self.p[u_indx, :])
		    delta_q = self.alpha * (2 * e * self.p[u_indx, :] - self.beta * self.q[i_indx, :])
		    # update with delata
		    self.p[u_indx, :] += delta_p
		    self.q[i_indx, :] += delta_q
            end = time.time() 
	    print 'iter time: ', end - start
            print 'iter'

    def learn(self):
        '''api'''
        self.load()
	self.kernel()

    def write_result(self):
        f1 = open('usr_factor.csv', 'w')
	f2 = open('item_factor.csv', 'w')
	for indx in xrange(len(self.usr_dct)):
	    f1.write('%s:' % self.reverse_usr_dct[indx])
	    for j in xrange(len(self.p[indx]) - 1):
	        f1.write('%s|' % self.p[indx][j])
	    f1.write(str(self.p[indx][len(self.p[indx]) - 1]) + '\n')
	for indx in xrange(len(self.item_dct)):
	    f2.write('%s:' % self.reverse_item_dct[indx])
	    for j in xrange(len(self.q[indx]) - 1):
	        f2.write('%s|' % self.q[indx][j])
	    f2.write(str(self.q[indx][len(self.q[indx]) - 1]) + '\n')

if __name__ == '__main__':
    bmf_solver = bmf(k = 80, rounds = 5, alpha = 0.005, beta = 0.02, train_fn = '/home/xunzhang/xunzhang/Data/mf/movielen1m', validate_fn = 'null', pred_fn = 'pred.csv', output = 'result.csv')
    bmf_solver.learn()
    bmf_solver.write_result()
    bmf_solver.predict_rating()
    bmf_solver.cal_rmse()
