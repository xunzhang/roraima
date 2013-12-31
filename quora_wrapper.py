from subprocess import Popen, PIPE

# topk must be same as in start_ functions
def quora_online(uid, p, topk = 50):
    p.stdin.write(uid + '\n')
    item_lst = []
    for i in xrange(topk):
        tmp = p.stdout.readline().strip('\n')
	if tmp == '-1':
	    return -1
        item_lst.append(tmp)
    return item_lst
        
def start_quora_online(usr_factor_fn, item_factor_fn, usr_blacklst_fn, artist_track_fn, topk = 50, cache_sz = 1000):
    scmd = './bin/quora_online --item_factor_file ' + item_factor_fn + \
    	' --usr_factor_file ' + usr_factor_fn + ' --usr_blacklst_file ' + usr_blacklst_fn + \
	' --artist_track_file ' + artist_track_fn + \
	' --topk ' + str(topk) + ' --cache_sz ' + str(cache_sz)
    p = Popen(scmd.split(), stdin = PIPE, stdout = PIPE)
    return p
