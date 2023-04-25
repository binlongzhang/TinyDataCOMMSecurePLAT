/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-25 02:47:30
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 05:58:54
 */
#include "CMysqlOP.h"
#include <iostream>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

CMysqlOP::CMysqlOP()
{
    // 初始化客户端库
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "could not initialize MySQL client library\n");
        exit(1);
    }	
	mysql_init(&mysql);
}


CMysqlOP::~CMysqlOP()
{
    mysql_library_end();
}

bool CMysqlOP::connectDB(std::string ip,std::string user,std::string pwd,std::string db)
{
	if (!mysql_real_connect(&mysql, ip.data(),user.data(),pwd.data(),db.data(), 0, NULL, 0))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
        // return mysql_errno(&mysql);
		return false;
    }

	return true;
}

int CMysqlOP::getKeyID()
{
	// 查询数据库
	const char * psql = {"select ikeysn from keysn for update"};
    if (mysql_query(&mysql, psql))
    {
        fprintf(stderr, "Failed to '%s': Error: %s\n", psql, mysql_error(&mysql));
        return mysql_errno(&mysql);
    }
	
	/*获取结果*/
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
    {
        fprintf(stderr, "Failed to get result: Error: %s\n", mysql_error(&mysql));
        return -1;
    }
	// 该表只有一条记录
	MYSQL_ROW row;
	int keyID = -1;
	if ((row = mysql_fetch_row(result)))
	{
		keyID = atoi(row[0]);
		// keyID = resSet->getInt(1);
	}
    mysql_free_result(result);

	return keyID;
}

// 秘钥ID在插入的时候回自动更新, 也可以手动更新
bool CMysqlOP::updataKeyID(int keyID)
{
	// 更新数据库
	std::string psql = "update keysn set ikeysn = " + std::to_string(keyID);
	if (mysql_query(&mysql, psql.data()))
    {
        fprintf(stderr, "Failed to '%s': Error: %s\n", psql.data(), mysql_error(&mysql));
        return false;
    }
	return true;
}

// 将生成的秘钥写入数据库
// 更新秘钥编号
bool CMysqlOP::writeSecKey(NodeSHMInfo *pNode)
{
	// 组织待插入的sql语句
	char psql[1024] = { 0 };
	sprintf(psql, "Insert Into seckeyinfo(clientid, serverid, keyid, createtime, state, seckey) \
					values ('%s', '%s', %d, DATE('%s') , %d, '%s') ", 
		pNode->clientID, pNode->serverID, pNode->seckeyID, 
		getCurTime().data(), 0, pNode->seckey);
	std::cout << "insert sql: " << psql << std::endl;

	if (mysql_query(&mysql, psql))
    {
        fprintf(stderr, "Failed to '%s': Error: %s\n", psql, mysql_error(&mysql));
        return false;
    }
	return true;
}

void CMysqlOP::closeDB()
{
	mysql_close(&mysql);
}

std::string CMysqlOP::getCurTime()
{
	time_t timep;
	time(&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));

	return tmp;
}
