#! /usr/bin/python

import os
import glob
import time
import socket
import datetime
import logging

from mpi4py import MPI

from DoubanAlg import gstore
from quora_wrapper import start_quora_online, quora_online

logging.basicConfig(filename='roraima_log', format = '%(asctime)s : %(levelname)s : %(message)s', level = logging.INFO)
logger = logging.getLogger(__name__)

log_path = '/logpath/'
query_dct = {}

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
	if l[0] == 'emtc' and l[4].startswith('dj_onlinefactor') or \
	    l[0] == 'playlist' and l[2].startswith('dj_onlinefactor'):
	    if l[1].isdigit(): 
	        ids.append(l[1])
    return ids

def local_store(uid, track_lst):
    timestamp = datetime.datetime.now()
    tmp_cmd = 'select * from user_top_rating_track_predict where user_id = %s;' % uid
    tmp = gstore.execute(tmp_cmd)
    if tmp:
        sql_cmd = 'update user_top_rating_track_predict set track_lst = "%s", time = "%s" where user_id = %s;' % (track_lst, timestamp, uid)
	logger.info(sql_cmd)
    else:
        sql_cmd  = 'insert into user_top_rating_track_predict (user_id, track_lst) values(%s, "%s");' % (uid, track_lst)
	logger.info(sql_cmd)
    gstore.execute(sql_cmd)
    gstore.commit()

def main(len_dct, p, topk):
    server_name = len_dct.keys()[0]
    fn = log_path + server_name + '/latest'
    if os.path.exists(fn):
	try:
	    f = file(fn)
            content = f.readlines()
	    #s = len_dct[server_name]
	    e = len(content)
	    if e > 1000:
	        s = e - 1000
	    else:
	        s = 0
	    if e > s:
	        len_dct[server_name] = e
		ids = load_uid(content[s : e])
		for uid in ids:
		    if query_dct.get(uid):
		        continue
		    query_dct[uid] = '0'
		    logger.info('uid:%s' % uid)
		    result = quora_online(uid, p, topk)
		    if result != -1: 
	                local_store(uid, ','.join(result))
            f.close()
	except:
	    logger.info('waiting')
    return len_dct

if __name__ == '__main__':
    comm = MPI.COMM_WORLD
    rk = comm.Get_rank()
    usr_factor_fn = 'usr_factor.csv'
    item_factor_fn = 'item_factor.csv'
    usr_blacklst_fn = 'usr_blacklst_dict.csv'
    artist_track_fn = 'artist_track_dict.csv'
    method = 'tree' # linear
    topk = 500
    cache_sz = 100
    p = start_quora_online(usr_factor_fn, item_factor_fn, usr_blacklst_fn, artist_track_fn, method, topk, cache_sz)
    if rk == 0:
        len_dct = {'dori' : 0}
    if rk == 1:
        len_dct = {'nori' : 0}
    if rk == 2:
        len_dct = {'thorin' : 0}
    logger.info('rank %s starting at main loop @%s' % (rk, socket.gethostname()))
    # main loop
    while 1:
        len_dct = main(len_dct, p, topk)
