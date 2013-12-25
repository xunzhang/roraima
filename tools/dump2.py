#! /usr/bin/python
import dpark
if __name__ == '__main__':
    rating_path = '/mfs/alg/tmp/jasontmp/user_song_rating_all/'
    sc = dpark.DparkContext()
    usr_track_dct = sc.textFile(rating_path).map(
    	    lambda x : (x.split()[0], x.split()[1])
	).uniq(
	).map(
	    lambda (x, y) : (x, [y])
	).reduceByKey(
	    lambda x, y : x + y
	).collectAsMap()
    f = file('/mfs/user/wuhong/tmp/usr_track_dct.csv', 'w')
    for uid, iid_lst in usr_track_dct.items():
        f.write('%s:%s' % (uid, iid_lst[0]))
	for iid in iid_lst[1:]:
	    f.write('|%s' % iid)
        f.write('\n')
    f.close()
