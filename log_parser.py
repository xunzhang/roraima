#! /usr/bin/python

import os
import glob
import time
import datetime

from mpi4py import MPI

from DoubanAlg import gstore
from quora_wrapper import start_quora_online, quora_online


#usr_heart_dct = {}
log_path = '/mfs/log/access-log/current/radiostat/'
answer_path = '/mfs/user/wuhong/tmp/'
#heart_path = '/mfs/user/wuhong/tmp/usr_heart_dict.csv'
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
	    l[4] == 'dj_onlinefactor':
	    if l[2].isdigit(): 
	        ids.append(l[2])
    return ids

def local_store(uid, track_lst):
    timestamp = datetime.datetime.now()
    tmp_cmd = 'select * from user_top_rating_track_predict where user_id = %s;' % uid
    tmp = gstore.execute(tmp_cmd)
    if tmp:
        sql_cmd = 'update user_top_rating_track_predict set track_lst = "%s", time = "%s" where user_id = %s;' % (track_lst, timestamp, uid)
	print sql_cmd
    else:
        sql_cmd  = 'insert into user_top_rating_track_predict (user_id, track_lst) values(%s, "%s");' % (uid, track_lst)
	print sql_cmd
    gstore.execute(sql_cmd)
    gstore.commit()

def main(len_dct, p, topk):
    #global usr_heart_dict
    server_name = len_dct.keys()[0]
    fn = log_path + server_name + '/radiostat_current'
    #fn = '/mfs/user/wuhong/tmp/radiostat_current'
    if os.path.exists(fn):
	try:
	    f = file(fn)
            content = f.readlines()
	    s = len_dct[server_name]
	    e = len(content)
	    if e > s:
	        len_dct[server_name] = e
		ids = load_uid(content[s : e])
		for uid in ids:
		    print uid
		    #if uid in usr_heart_dict: # heart > 10
		    result = quora_online(uid, p, topk)
		    if result != -1: 
			local_store(uid, ','.join(result))
            f.close()
	except:
	    print 'waiting'
    return len_dct

if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    rk = comm.Get_rank()
    usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv'
    item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv'
    usr_heart_fn = '/mfs/user/wuhong/tmp/usr_heart_dict.csv'
    topk = 500
    cache_sz = 10
    p = start_quora_online(usr_factor_fn, item_factor_fn, usr_heart_fn, topk, cache_sz)
    #f_heart = file(heart_path)
    #for l in f_heart:
    #    usr_heart_dct[l.strip('\n').split(':')[0]] = 1
    #f_heart.close()
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
