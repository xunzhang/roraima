if __name__ == '__main__':
    f1 = file('/mfs/user/wuhong/Data/fm/factor_100d/new_usr_factor.csv')
    f2 = file('/mfs/user/wuhong/Data/fm/factor_100d/usr_factor.csv', 'w')
    for line in f1:
        l = line.strip('\n').split(':')[0]
	if l.isdigit():
	    f2.write(line)

