DROP TABLE IF EXISTS `user_top_rating_track_predict1`;
CREATE TABLE `user_top_rating_track_predict1` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL DEFAULT '0',
    `track_id` mediumint(8) NOT NULL DEFAULT '0',
    `rank` int(4) NOT NULL DEFAULT '0',
    `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    UNIQUE KEY (`user_id`, `track_id`),
    KEY `user_id_id` (`user_id`, `id`) 
) ENGINE=InnoDB AUTO_INCREMENT = 1 DEFAULT CHARSET=latin1;
