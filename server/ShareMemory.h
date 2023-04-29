/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-20 12:21:33
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-27 12:26:51
 */
#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
const char RandX = 'x';
class ShareMemory
{
public:
	ShareMemory(int key);
	ShareMemory(int key, int size);
	ShareMemory(const char* name);
	ShareMemory(const char* name, int size);
	virtual ~ShareMemory();

	void* mapShm();
	int unmapShm();
	int delShm();


private:
	int getShmID(key_t key, int shmSize, int flag);

private:
	int m_shmID;
	void* m_shmAddr = NULL;

};