from quora_wrapper import start_quora_online, quora_online, start_quora_brute_force, quora_brute_force

def foo(uid, p):
    print quora_online(uid, p, 50)

def foo2(uid, p):
    print quora_brute_force(uid, p, 50)

if __name__ == '__main__':
    usr_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv'
    item_factor_fn = '/mfs/user/wuhong/Data/fm/factor_100d/item_factor.csv'
    topk = 50
    cache_sz = 10000
    p = start_quora_online(usr_factor_fn, item_factor_fn, topk, cache_sz)
    p2 = start_quora_brute_force(usr_factor_fn, item_factor_fn, topk, cache_sz)
    print '-----------------'
    foo2('58106680', p2)
    print '-----------------'
    foo2('VwF6KBl/Ne8', p2)
    foo('58106680', p)
    print '-----------------'
    foo('VwF6KBl/Ne8', p)
