#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "SecKeyShm.h"

SecKeyShm::SecKeyShm(int key):ShareMemory(key){}

SecKeyShm::SecKeyShm(const char* pathName): ShareMemory(pathName){}

SecKeyShm::SecKeyShm(int key, int maxNode):
	ShareMemory(key, sizeof(int) + maxNode*sizeof(NodeSHMInfo)), m_maxNode(maxNode)
{
	//	调用了默认构造函数，没有进行共享内存映射，在此进行映射
	void *p = mapShm();
	//	初始化共享内存
	memset(p, 0x00, sizeof(int) + maxNode*sizeof(NodeSHMInfo));
	//	将secret key头初始化
	memcpy(p, &m_maxNode, sizeof(int));
	//	释放共享内存
	unmapShm();
}



SecKeyShm::SecKeyShm(const char* pathName, int maxNode):
	ShareMemory(pathName, sizeof(int) + maxNode*sizeof(NodeSHMInfo)), m_maxNode(maxNode)
{
	//	调用了默认构造函数，没有进行共享内存映射，在此进行映射
	void *p = mapShm();
	//	初始化共享内存
	memset(p, 0x00, sizeof(int) + maxNode*sizeof(NodeSHMInfo));
	//	将secret key头初始化
	memcpy(p, &m_maxNode, sizeof(int));
	//	释放共享内存
	unmapShm();
}

SecKeyShm::~SecKeyShm(){}

int SecKeyShm::shmWrite(NodeSHMInfo* pNodeInfo)
{
	//	获得共享内存
	void *p = mapShm();
	
	// 取出节点数
	memcpy(&m_maxNode, p, sizeof(int));
	std::cout << "maxNode:" << m_maxNode << std::endl;
	
	// 重置NodeSHMInfo指针
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p+sizeof(int));
	
	// 寻找已经被存储在共享内存中的
	int i = 0;
	for(i=0; i<m_maxNode; i++){
		if(strcmp(pNodeInfo->clientID, pNode[i].clientID)==0 &&
		   strcmp(pNodeInfo->serverID, pNode[i].serverID)==0){

			memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
			break;
		}
	}
	
	//	寻找空位置并将该节点信息写入
	NodeSHMInfo tmp;
	memset(&tmp, 0x00, sizeof(NodeSHMInfo));
	if(i==m_maxNode){
		for(i=0; i<m_maxNode; i++)
		if(memcmp(&pNode[i], &tmp, sizeof(NodeSHMInfo))==0) {
			memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
			break;
		}
	}
	
	//	释放共享内存
	unmapShm();
	
	//	内存慢
	if(i==m_maxNode){
		std::cout << "no space to use" << std::endl;
		return -1;
	}
	
	return 0;
}

int SecKeyShm::shmRead(const char* clientID, const char* serverID, NodeSHMInfo* pNodeInfo)
{
	//	获取共享资源
	void *p = mapShm();
	
	//	获取节点数
	memcpy(&m_maxNode, p, sizeof(int));
	std::cout << "maxNode:" << m_maxNode << std::endl;
	
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p+sizeof(int));
	
	int i = 0;
	for(i=0; i<m_maxNode; i++){
		if(strcmp(clientID, pNode[i].clientID)==0 &&
		   strcmp(serverID, pNode[i].serverID)==0){
			memcpy(pNodeInfo, &pNode[i], sizeof(NodeSHMInfo));
			break;
		}
	}
	
	//	释放共享资源
	unmapShm();
	
	//	没找到对应的信息
	if(i==m_maxNode)
	{
		std::cout << "not found" << std::endl;
		return -1;
	}
	
	return 0;
}

void SecKeyShm::printShm()
{
	//	获取共享内存
	void *p = mapShm();
	
	//	获取节点数
	memcpy(&m_maxNode, p, sizeof(int));
	std::cout << "maxNode:" << m_maxNode << std::endl;
	
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p+sizeof(int));
	
	int i = 0;
	for(i=0; i<m_maxNode; i++)
	{
		std::cout << "-------" << i << "-------" << std::endl;
		std::cout << "status:" << pNode[i].status << std::endl;
		std::cout << "seckeyID:" << pNode[i].seckeyID << std::endl;
		std::cout << "clientID:" << pNode[i].clientID << std::endl;
		std::cout << "serverID:" << pNode[i].serverID << std::endl;
		std::cout << "seckey:" << pNode[i].seckey << std::endl;
	}
	
	return;
}
