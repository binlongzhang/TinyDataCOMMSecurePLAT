/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-18 08:44:23
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 04:41:59
 */
#pragma once
#include "ShareMemory.h"
#include "NodeSHMInfo.h"

// 当前版本以数组的形式存储，头存储了数组长度
class SecKeyShm : public ShareMemory
{
public:
	SecKeyShm(int key);
	SecKeyShm(int key, int maxNode);
	SecKeyShm(const char* pathName);
	SecKeyShm(const char* pathName, int maxNode);
	~SecKeyShm();

	int shmWrite(NodeSHMInfo* pNodeInfo);
	int shmRead(const char* clientID, const char* serverID, NodeSHMInfo* pNodeInfo);

	void printShm();
private:
	int m_maxNode;
};


