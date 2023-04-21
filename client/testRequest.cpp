#include <iostream>
#include "RequestCodec.h"
#include "CodecFactory.h"
#include "RequestFactory.h"

using namespace std;


// int main()
// {
// #if 1
// 	// 编码
// 	RequestMsg req;
// 	req.cmdType = 100;
// 	strcpy(req.clientId, "hello");
// 	strcpy(req.authCode, "1111");
// 	strcpy(req.serverId, "22222");
// 	strcpy(req.r1, "abcdefg");

// 	char* outData;
// 	int len;

// 	//使用工厂类构造编解码对象
// 	//1-new子工厂类对象
// 	CodecFactory *factory = new RequestFactory(&req);
// 	Codec *codec = factory->createCodec();
// 	codec->msgEncode(&outData, len);
// 	delete factory;
// 	delete codec;

// 	RequestMsg* tmp;
// 	factory = new RequestFactory();
// 	codec = factory->createCodec();
// 	tmp = (RequestMsg *)codec->msgDecode(outData, len);

// 	cout << "cmdtype: " << tmp->cmdType << endl;
// 	cout << "serverID: " << tmp->serverId << endl;
// 	cout << "clientID: " << tmp->clientId << endl;
// 	cout << "r1: " << tmp->r1 << endl;

// #endif

// #if 0
// //测试RequestCodec类

// 	// 编码
// 	RequestMsg req;
// 	req.cmdType = 100;
// 	strcpy(req.clientId, "hello");
// 	strcpy(req.authCode, "1111");
// 	strcpy(req.serverId, "22222");
// 	strcpy(req.r1, "abcdefg");

// 	Codec* codec = new RequestCodec(&req);
// 	// 调用编码函数
// 	char* outData;
// 	int len;
// 	codec->msgEncode(&outData, len);

// 	// ========================
// 	// 解码
// 	codec = new RequestCodec();
// 	RequestMsg* tmp = (RequestMsg*)codec->msgDecode(outData, len);
// 	cout << "cmdtype: " << tmp->cmdType << endl;
// 	cout << "serverID: " << tmp->serverId << endl;
// 	cout << "clientID: " << tmp->clientId << endl;
// 	cout << "r1: " << tmp->r1 << endl;


// #endif
// 	return 0;
// }