/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-18 23:14:42
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 04:41:07
 */
#pragma once
#include "TcpSocket.h"
#include "SecKeyShm.h"
#include "NodeSHMInfo.h"
#include "CodecFactory.h"
#include "RequestFactory.h"
#include "RespondFactory.h"

class ClientInfo
{
public:
	char clinetID[12];			// 客户端ID
	char serverID[12];			// 服务器ID
	//char authCode[65];			// 消息认证码
	char serverIP[32];			// 服务器IP
	unsigned short serverPort;	// 服务器端口
	int maxNode;				// 共享内存节点个数
	int shmKey;					// 共享内存的Key
};

class ClientOperation
{
public:
	ClientOperation(ClientInfo *info);
	~ClientOperation();

	// 秘钥协商
	int secKeyAgree();
	// 秘钥校验
	int secKeyCheck() {return 0;}
	// 秘钥注销
	int secKeyRevoke() {return 0;}
	// 秘钥查看
	int secKeyView() {return 0;}

private:
	void getRandString(int len, char* randBuf);

private:
	ClientInfo m_info;
	TcpSocket m_socket;
	SecKeyShm* m_shm;
	// 用于保护共享内存
	pthread_rwlock_t shmem_wlock;
};

