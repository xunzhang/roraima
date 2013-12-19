from quora_wrapper import start_quora, quora_online

def foo(uid):
    print quora_online(uid, p, 30)

if __name__ == '__main__':
    p = start_quora()
    foo('58106680')
    print '-----------------'
    foo('VwF6KBl/Ne8')
    
