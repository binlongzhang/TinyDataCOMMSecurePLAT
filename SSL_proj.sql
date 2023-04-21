/*
Navicat MySQL Data Transfer

Source Server         : 192.168.78.251-bj24
Source Server Version : 50620
Source Host           : 192.168.78.251:3306
Source Database       : mydb66

Target Server Type    : MYSQL
Target Server Version : 50620
File Encoding         : 65001

Date: 2017-10-10 09:53:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for keysn
-- ----------------------------
DROP TABLE IF EXISTS `keysn`;
CREATE TABLE `keysn` (
  `ikeysn` int(12) NOT NULL,
  PRIMARY KEY (`ikeysn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of keysn
-- ----------------------------
INSERT INTO `keysn` VALUES ('1');

-- ----------------------------
-- Table structure for seckeyinfo
-- ----------------------------
DROP TABLE IF EXISTS `seckeyinfo`;
CREATE TABLE `seckeyinfo` (
  `clientid` int(4) DEFAULT NULL,
  `serverid` int(4) DEFAULT NULL,
  `keyid` int(9) NOT NULL,
  `createtime` date DEFAULT NULL,
  `state` int(4) DEFAULT NULL,
  `seckey` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`keyid`),
  KEY `seckeynode_serverid_fk` (`serverid`),
  KEY `IX_seckeyinfo_clientid` (`clientid`),
  CONSTRAINT `seckeynode_clientid_fk` FOREIGN KEY (`clientid`) REFERENCES `secnode` (`id`),
  CONSTRAINT `seckeynode_serverid_fk` FOREIGN KEY (`serverid`) REFERENCES `secnode` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of seckeyinfo
-- ----------------------------

-- ----------------------------
-- Table structure for secnode
-- ----------------------------
DROP TABLE IF EXISTS `secnode`;
CREATE TABLE `secnode` (
  `id` int(4) NOT NULL,
  `name` varchar(512) NOT NULL,
  `nodedesc` varchar(512) DEFAULT NULL,
  `createtime` date DEFAULT NULL,
  `authcode` int(12) DEFAULT NULL,
  `state` int(4) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of secnode
-- ----------------------------
INSERT INTO `secnode` VALUES ('1', '网银中心', '北京金融街23号', '2018-06-16', '1', '0');
INSERT INTO `secnode` VALUES ('1111', '深圳分中心', '广州天河金融', '2016-06-16', '1111', '0');

-- ----------------------------
-- Table structure for srvcfg
-- ----------------------------
DROP TABLE IF EXISTS `srvcfg`;
CREATE TABLE `srvcfg` (
  `key` varchar(64) DEFAULT NULL,
  `valude` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of srvcfg
-- ----------------------------
INSERT INTO `srvcfg` VALUES ('secmng_server_ip', '192.168.1.1');

-- ----------------------------
-- Table structure for tran
-- ----------------------------
DROP TABLE IF EXISTS `tran`;
CREATE TABLE `tran` (
  `iID` int(12) NOT NULL,
  `tran_operator` int(12) DEFAULT NULL,
  `trantime` date DEFAULT NULL,
  `tranid` int(4) DEFAULT NULL,
  `trandesc` varchar(512) DEFAULT NULL,
  PRIMARY KEY (`iID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of tran
-- ----------------------------
SET FOREIGN_KEY_CHECKS=1;
