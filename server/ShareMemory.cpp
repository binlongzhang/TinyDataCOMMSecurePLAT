#include <sys/ipc.h>
#include <sys/shm.h>
#include "ShareMemory.h"

// 一些列获取共享内存的构造函数
ShareMemory::ShareMemory(int key)
{
	m_shmID = getShmID(key, 0, 0);
}

ShareMemory::ShareMemory(int key, int size)
{
	m_shmID = getShmID(key, size, IPC_CREAT | 0755);
}

ShareMemory::ShareMemory(const char* name)
{
	key_t key = ftok(name, RandX);
	m_shmID = getShmID(key, 0, 0);
}

ShareMemory::ShareMemory(const char* name, int size)
{
	key_t key = ftok(name, RandX);
	m_shmID = getShmID(key, size, IPC_CREAT | 0755);
}

int ShareMemory::getShmID(key_t key, int size, int flag)
{
	m_shmID = shmget(key, size, flag);
	if(m_shmID<0)
	{
		//错误日志
		return -1;
	}
	return m_shmID;
}

ShareMemory::~ShareMemory()
{

}


// 功能函数

//	映射key值对应的内存并将内存地址保存在m_shmAddr，并将其地址返回
void *ShareMemory::mapShm()
{
	m_shmAddr = shmat(m_shmID, NULL, 0);
	if(m_shmAddr==(void *)-1)
	{
		//	错误日志
		return NULL;
	}
	return m_shmAddr;
}

int ShareMemory::unmapShm()
{
	int ret = shmdt(m_shmAddr);
	return ret;
}

int ShareMemory::delShm()
{
	int ret = shmctl(m_shmID, IPC_RMID, NULL);
	return ret;
}