/******************************************/
/*   DatabaseName = gymmngsys   */
/*   TableName = features   */
/******************************************/
CREATE TABLE `features` (
  `gymid` char(11) NOT NULL,
  `fitness` char(1) DEFAULT '0',
  `basketball` char(1) DEFAULT '0',
  `badminton` char(1) DEFAULT '0',
  `swimming` char(1) DEFAULT '0',
  `bath` char(1) DEFAULT '0',
  `contest` char(1) DEFAULT '0',
  PRIMARY KEY (`gymid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
;

/******************************************/
/*   DatabaseName = gymmngsys   */
/*   TableName = gyminfo   */
/******************************************/
CREATE TABLE `gyminfo` (
  `gymid` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `gymname` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `position` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `builttime` date DEFAULT NULL,
  `profit` int(10) DEFAULT '0',
  `password` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `uprice` int(10) DEFAULT '0',
  PRIMARY KEY (`gymid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
;

/******************************************/
/*   DatabaseName = gymmngsys   */
/*   TableName = personinfo   */
/******************************************/
CREATE TABLE `personinfo` (
  `uid` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `name` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `gender` char(1) DEFAULT '0',
  `age` int(10) DEFAULT '0',
  `st_time` datetime DEFAULT NULL,
  `balance` int(10) DEFAULT '0',
  `gymid` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`gymid`,`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8
;

/******************************************/
/*   DatabaseName = gymmngsys   */
/*   TableName = repairlog   */
/******************************************/
CREATE TABLE `repairlog` (
  `gymid` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `time` date DEFAULT NULL,
  `reason` varchar(100) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `spend` int(10) DEFAULT NULL,
  `uid` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8
;
