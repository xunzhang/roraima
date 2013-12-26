
input_path = '/mfs/alg/rivendb/final/alg/fm/user_song_rating_factor_model_all/'
output_path = '/mfs/user/wuhong/Data/fm/'

if __name__ == '__main__':

  from dpark import DparkContext
  dpark = DparkContext()

  usr_factor_dct = {}
  item_factor_dct = {} 

  usr_factor_lst = dpark.beansdb(input_path).filter(
  	    lambda (k, (v, x1, x2)) : k.startswith('u')
  	).map(
	    lambda (k, (v, x1, x2)) : (k[1:], v)
	).collect()
  for item in usr_factor_lst:
      tmp = item[1][1]
      tmp.insert(0, item[1][0])
      usr_factor_dct[item[0]] = tmp
  
  item_factor_lst = dpark.beansdb(input_path).filter(
  	    lambda (k, (v, x1, x2)) : k.startswith('t')
  	).map(
	    lambda (k, (v, x1, x2)) : (k[1:], v)
	).collect()
  for item in item_factor_lst:
      tmp = item[1][1]
      tmp.insert(0, item[1][0])
      item_factor_dct[item[0]] = tmp

  f1 = file(output_path + 'new_usr_factor.csv', 'w')
  f2 = file(output_path + 'new_item_factor.csv', 'w')
  
  for k, v in usr_factor_dct.items():
      f1.write('%s:%s' % (k, v[0]))
      for fac in v[1:]:
          f1.write('|%s' % fac)
      f1.write('\n')
  for k, v in item_factor_dct.items():
      f2.write('%s:%s' % (k, v[0]))
      for fac in v[1:]:
          f2.write('|%s' % fac)
      f2.write('\n')
  f1.close()
  f2.close()
