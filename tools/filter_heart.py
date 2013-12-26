#! /usr/bin/python
import dpark
if __name__ == '__main__':
    rating_path = '/mfs/alg/dbsync/user_favorite_track/'
    sc = dpark.DparkContext()
    usr_track_dct = sc.textFile(rating_path).map(
    	    lambda x : (x.split()[0], x.split()[1])
	).uniq(100
	).map(
	    lambda (x, y) : (x, [y])
	).reduceByKey(
	    lambda x, y : x + y, 100
	).filter(lambda (x, y) : len(y) > 10).mapValue(lambda x : '|'.join(x)).map(lambda (x, y) : x + ':' + y)
    usr_track_dct.saveAsCSVFile('/mfs/user/wuhong/tmp/usr_heart_dct.csv')
    #f = file('/mfs/user/wuhong/tmp/usr_heart_dct.csv', 'w')
    #for uid, iid_lst in usr_track_dct.items():
    #    f.write('%s:%s' % (uid, iid_lst[0]))
    #	for iid in iid_lst[1:]:
    #	    f.write('|%s' % iid)
    #    f.write('\n')
    #f.close()
