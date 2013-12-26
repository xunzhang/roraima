from subprocess import Popen, PIPE

# topk must be same as in start_ functions
def quora_brute_force(uid, p, topk = 50):
    p.stdin.write(uid + '\n')
    item_lst = []
    for i in xrange(topk):
        item_lst.append(p.stdout.readline().strip('\n'))
    return item_lst

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
        
def start_quora_brute_force(usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv', item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv', topk = 50, cache_sz = 10000):
    scmd = './bin/quora_brute_force --item_factor_file ' + item_factor_fn + ' --usr_factor_file ' + usr_factor_fn + ' --topk ' + str(topk) + ' --cache_sz ' + str(cache_sz)
    p = Popen(scmd.split(), stdin = PIPE, stdout = PIPE)
    return p

def start_quora_online(usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv', item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv', usr_heart_fn = '/mfs/user/wuhong/tmp/usr_heart_dict.csv', topk = 50, cache_sz = 10000):
    scmd = './bin/quora_online --item_factor_file ' + item_factor_fn + ' --usr_factor_file ' + usr_factor_fn + ' --usr_heart_file ' + usr_heart_fn + ' --topk ' + str(topk) + ' --cache_sz ' + str(cache_sz)
    p = Popen(scmd.split(), stdin = PIPE, stdout = PIPE)
    return p
