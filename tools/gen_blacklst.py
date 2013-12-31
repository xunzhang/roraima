# -*- coding:utf-8 -*-
import sys
sys.path.append('/var/shire')
from glob import glob
from dpark import DparkContext
from fm.model.alg.radio_object import RadioUser, RadioArtist

dpark = DparkContext()
FAV_CNT_THR = 20

# user favorite tracks
files = glob(r'/mfs/alg/dbsync/user_favorite_track/*')
user_fav_rdd = dpark.textFile(files, splitSize=64<<20).map(
            lambda line: line.strip().split('\t')
        ).filter(
            # uid, tid, sid
            lambda val_list: len(val_list)==3 and all([val.isdigit() \
                    for val in val_list])
        ).map(
            lambda (uid, tid, sid): (int(uid), (tid))
        ).groupByKey(100).filter(
            lambda (uid, fav_track_list): len(fav_track_list) >= FAV_CNT_THR
        )
seed_user_list = user_fav_rdd.map(
            lambda (uid, fav_track_list): uid
        ).collect()
dseed_user_set = dpark.broadcast(set(seed_user_list))

# user ban tracks
files = glob(r'/mfs/alg/dbsync/user_banned_track2/*')
user_ban_rdd = dpark.textFile(files, splitSize=64<<20).map(
            lambda line: line.strip().split('\t')
        ).filter(
            # uid, sid, timestamp
            lambda val_list: len(val_list)==3 and all([val.isdigit() \
                    for val in val_list[:2]]) and (int(val_list[0]) \
                    in dseed_user_set.value)
        ).map(
            lambda (uid, sid, timestamp): (int(uid), int(sid))
        ).groupByKey(100)

# merge fav & ban tracks
def _merge_track(fav_track_list, ban_track_list):
    if fav_track_list and ban_track_list:
        return fav_track_list + ban_track_list
    else:
        return fav_track_list if fav_track_list else ban_track_list
user_fav_rdd = user_fav_rdd.outerJoin(user_ban_rdd, 100).mapValue(
            lambda (fav_list, ban_list): _merge_track(fav_list, ban_list)
        )

# track -> artist info
track_artist_dict = dpark.textFile('/mfs/alg/radio/song_info_v2.data',
        splitSize=4<<20).map(
            lambda line: line.strip().split('\t')
        ).map(
            # tid, new_id, aids, sids, main_subject_id, is_hidden, artist_name, song_name
            lambda val_list: (int(val_list[0]), [int(aid) for aid in
                val_list[2].split(',')] if val_list[2] else [])
        ).collectAsMap()
dtrack_artist_dict = dpark.broadcast(track_artist_dict)

# ban artist tracks
def ban_artist(uid, ban_track_list):
    fav_artist_set = set([int(tid) for tid, _ in \
            RadioUser.get_user_favorite_artists(uid)])
    ban_artist_cnt_dict = {}
    for tid in ban_track_list:
        aid_list = dtrack_artist_dict.value.get(tid, [])
        for aid in aid_list:
            ban_artist_cnt_dict[aid] = ban_artist_cnt_dict.get(aid, 0) + 1
    fin_artist_set = set()
    for aid, cnt in ban_artist_cnt_dict.items():
        if (cnt >= 3) and (aid not in fav_artist_set):
            fin_artist_set.add(aid)
    return (uid, fin_artist_set)
user_ban_artist_rdd = user_ban_rdd.map(
            lambda (uid, ban_track_list): ban_artist(uid, ban_track_list)
        )
banned_artist_list = user_ban_artist_rdd.flatMap(
            lambda (uid, ban_artist_set): ban_artist_set
        ).uniq().collect()
banned_artist_set = set(banned_artist_list)
output_file = open('artist_track_dict.csv', 'w')
for aid in banned_artist_set:
    artist_obj = RadioArtist.get(aid)
    if artist_obj:
        sid_str = '|'.join(map(str, artist_obj.song_ids))
        print >> output_file, '%s:%s' % (aid, sid_str)
output_file.close()

# merge artists
def _merge_artist(fav_track_list, ban_artist_set):
    if fav_track_list and not ban_artist_set:
        return map(str, fav_track_list)
    elif not fav_track_list and ban_artist_set:
        return ['a' + str(aid) for aid in ban_artist_set]
    elif fav_track_list and ban_artist_set:
        return map(str, fav_track_list) + ['a' + str(aid) for aid in ban_artist_set]
user_banned_rdd = user_fav_rdd.outerJoin(user_ban_artist_rdd, 100).mapValue(
            lambda (fav_list, ban_list): _merge_artist(fav_list, ban_list)
        ).map(
            lambda (uid, res_list): '%s:%s' % (uid, '|'.join(res_list))
        ).saveAsCSVFile('blacklst.csv')
