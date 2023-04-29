/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-20 12:21:33
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 09:36:40
 */
#pragma once
#include "Codec.h"

struct RespondMsg
{
	enum RV
	{
		Success = 0,
		DataPassErr,
		ClientNoExist,
		ServerIDError
	};

	int rv;			   // 返回值
	char clientId[12]; // 客户端编号
	char serverId[12]; // 服务器编号
	char r2[64];	   // 服务器端随机数
	int seckeyid;	   // 对称密钥编号    keysn
	RespondMsg() {}
	RespondMsg(char *clientID, char *serverID, char *r2, int rv, int seckeyID)
	{
		this->rv = rv;
		this->seckeyid = seckeyid;
		strcpy(this->clientId, clientID);
		strcpy(this->serverId, serverID);
		strcpy(this->r2, r2);
	}
};

class RespondCodec : public Codec
{
public:
	RespondCodec();
	RespondCodec(RespondMsg *msg);
	~RespondCodec();

	// 函数重载
	int msgEncode(char **outData, int &len);
	void *msgDecode(char *inData, int inLen);

private:
	RespondMsg m_msg;
};
