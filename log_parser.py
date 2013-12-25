#! /usr/bin/python

import os
import glob
import time
import datetime

from mpi4py import MPI

from quora_wrapper import start_quora_online, quora_online

log_path = '/mfs/log/access-log/current/radiostat/'
answer_path = '/mfs/user/wuhong/tmp/'
server_list = ['aragorn', 'dori', 'nori', 'thorin']

def exist_check(fn):
    return os.path.exists(fn)

def due_check(fn):
    t = os.path.getmtime(fn)
    date = datetime.datetime.fromtimestamp(t)
    delta = datetime.datetime.now() - date
    threshold = delta.days * 3600 * 24 + delta.seconds
    if threshold > 600:
        return False
    return True

def load_uid(llst):
    ids = []
    for line in llst:
        l = line.strip('\n').split(',')
	if l[0] == 'emtc' and \
	    l[4] == 'dj_onlinefac':
	    if l[2].isdigit(): 
	        ids.append(l[2])
    return ids

def main(len_dct, p, topk):
    server_name = len_dct.keys()[0]
    #fn = log_path + server_name + '/radiostat_current'
    fn = '/mfs/user/wuhong/tmp/radiostat_current'
    if os.path.exists(fn):
        #with open(fn) as f:
	try:
	    f = file(fn)
            content = f.readlines()
	    s = len_dct[server_name]
	    e = len(content)
	    if e > s:
	        len_dct[server_name] = e
		ids = load_uid(content[s : e])
		print ids
		for uid in ids:
		    result = quora_online(uid, p, topk)
		    print result
            f.close()
	except:
	    print 'waiting'
    return len_dct

if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    rk = comm.Get_rank()
    usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv'
    item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv'
    topk = 20
    cache_sz = 10
    p = start_quora_online(usr_factor_fn, item_factor_fn, topk, cache_sz)
    if rk == 0:
        len_dct = {'dori' : 0}
    if rk == 1:
        len_dct = {'aragorn' : 0}
    if rk == 2:
        len_dct = {'nori' : 0}
    if rk == 3:
        len_dct = {'thorin' : 0}
    # main loop
    while 1:
        len_dct = main(len_dct, p, topk)
