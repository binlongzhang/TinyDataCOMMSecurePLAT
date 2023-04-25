/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-25 02:40:42
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 05:01:05
 */
#pragma once
#include <string>
#include <mysql/mysql.h>
#include "SecKeyShm.h"

class CMysqlOP
{
public:
	CMysqlOP();
	~CMysqlOP();

	// 初始化环境连接数据库
	bool connectDB(std::string ip,std::string user,std::string pwd,std::string db);
	// 得到keyID
	int getKeyID();
	bool updataKeyID(int keyID);
	bool writeSecKey(NodeSHMInfo *pNode);
	void closeDB();

private:
	// 获取当前时间, 并格式化为字符串
	std::string getCurTime();

private:
	MYSQL mysql;
};
