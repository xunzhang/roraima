#! /usr/bin/python
import sys

from mpi4py import MPI
from dpark import DparkContext

from parasol.loader import loader
from quora_wrapper import start_quora_online, quora_online

if __name__ == '__main__':
    from mpi4py import MPI
    comm = MPI.COMM_WORLD
    rk = comm.Get_rank()
    usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv' 
    item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv'
    topk = 200
    cache_sz = 10
    p = start_quora_online(usr_factor_fn, item_factor_fn, topk, cache_sz) 
    ld = loader(usr_factor_fn, comm)
    lines = ld.load()
    print len(lines)
    result_dct = {}
    for line in lines:
        uid = line.split(':')[0]
	result_dct[uid] = quora_online(uid, p, topk)
    sc = DparkContext()
    rdd = sc.makeRDD(zip(result_dct.keys(), result_dct.values()), 100)
    rdd.saveAsBeansdb('/mfs/user/wuhong/tmp/fm/max_dot_product_top200/_' + str(rk))
