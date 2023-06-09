﻿/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-20 20:51:28
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 08:54:16
 */
#pragma once
#include "TcpSocket.h"
#include "SecKeyShm.h"
#include "TcpServer.h"
#include "RequestCodec.h"
#include "CMysqlOP.h"
#include <pthread.h>
#include <map>
#include "RequestFactory.h"
#include <string>
#include <iostream>
#include "RespondFactory.h"
#include "NodeSHMInfo.h"
class ServerInfo
{
public:
	char serverID[12]; // 服务器端编号
	char dbUse[24];	   // 数据库用户名
	char dbPasswd[24]; // 数据库密码
	char dbSID[24];	   // 数据库sid

	unsigned short sPort; // 服务器绑定的端口
	int maxnode;		  // 共享内存最大网点树 客户端默认1个
	int shmkey;			  // 共享内存keyid 创建共享内存时使用
};

void *wroking(void *arg);

class ServerOperation
{
public:
	ServerOperation(ServerInfo *info, std::string dbIP, std::string dbUser, std::string dbPWD, std::string dbName);
	~ServerOperation();

	// 服务器开始工作
	void startWork();
	// 秘钥协商
	int secKeyAgree(RequestMsg *reqmsg, char **outData, int &outLen);
	// 秘钥校验
	int secKeyCheck();
	// 秘钥注销
	int secKeyRevoke();
	// 秘钥查看
	int secKeyView();

	// friend void* wroking(void * arg);
	// 线程回调也可使用静态成员函数
	static void *wrokingHard(void *arg);
	static void catchSignal(int num);

	//	sockeList共享资源访问控制
	void socketListLock();
	void socketListUnLock();

private:
	void getRandString(int len, char *randBuf);
	bool vaildRequest(RequestMsg *reqMsg, char **outData, int &outLen);

private:
	ServerInfo m_info;
	SecKeyShm *m_shm;
	TcpServer m_server;
	TcpSocket *m_client;
	CMysqlOP m_databaseOP;
	std::map<pthread_t, TcpSocket *> m_listSocket;
	// pthread_mutex_t m_socketListLock = PTHREAD_MUTEX_INITIALIZER;
	static bool m_stop;
	// 用于保护到来的连接请求队列
	pthread_spinlock_t m_spinlock_SocketMap;
	// 用于保护共享内存
	pthread_rwlock_t shmem_wlock;
};