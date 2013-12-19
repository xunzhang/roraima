from subprocess import Popen, PIPE

def start_quora():
    p = Popen(['./a.out'], stdin = PIPE, stdout = PIPE)
    return p

def quora_online(uid, p, topk = 10):
    p.stdin.write(uid + '\n')
    item_lst = []
    for i in xrange(topk):
        item_lst.append(p.stdout.readline().strip('\n'))
    return item_lst
        
