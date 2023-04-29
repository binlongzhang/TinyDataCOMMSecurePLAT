#include "ClientOperation.h"
#include "RequestCodec.h"
#include <string.h>
#include <time.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <iostream>
#include <openssl/evp.h>

using namespace std;

ClientOperation::ClientOperation(ClientInfo * info)
{
	memcpy(&m_info, info, sizeof(ClientInfo)); 
	//创建共享内存
	m_shm = new SecKeyShm(m_info.shmKey, m_info.maxNode);

		// 初始化共享内存所需的读写锁
	pthread_rwlockattr_t rw_att;
	pthread_rwlockattr_init(&rw_att);
    pthread_rwlockattr_setkind_np(&rw_att, PTHREAD_RWLOCK_PREFER_WRITER_NP);
    pthread_rwlockattr_setpshared(&rw_att, PTHREAD_PROCESS_SHARED);
    pthread_rwlock_init(&shmem_wlock, &rw_att);
    pthread_rwlockattr_destroy(&rw_att);
}

ClientOperation::~ClientOperation()
{
	pthread_rwlock_destroy(&shmem_wlock);
}


int ClientOperation::secKeyAgree()
{
	//准备请求数据 
	RequestMsg req;
	memset(&req, 0x00, sizeof(RequestMsg));
	req.cmdType = RequestCodec::NewOrUpdate;
	strcpy(req.clientId, m_info.clinetID);
	strcpy(req.serverId, m_info.serverID);
	getRandString(sizeof(req.r1), req.r1);
	//使用hmac函数生成哈希值----消息认证码
	char key[64];
	unsigned int len;
	unsigned char md[SHA256_DIGEST_LENGTH];
	memset(key, 0x00, sizeof(key));
	sprintf(key, "@%s+%s@", req.serverId, req.clientId);
	HMAC(EVP_sha256(), key, strlen(key), (unsigned char *)req.r1, strlen(req.r1), md, &len);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		sprintf(&req.authCode[2 * i], "%02x", md[i]);
	}

	cout << "---------------------------------------" << endl;
	cout << "key:" << key << endl;
	cout << "r1:" << req.r1 << endl;
	cout << "authCode:" << req.authCode << endl;
	cout << "---------------------------------------" << endl;


	//将要发送的数据进行编码
	int dataLen;
	char *outData = NULL;
	CodecFactory *factory = new RequestFactory(&req);
	Codec *pCodec = factory->createCodec();
	pCodec->msgEncode(&outData, dataLen);
	delete factory;
	delete pCodec;

	//连接服务端
	if(m_socket.connectToHost(m_info.serverIP, m_info.serverPort)!=0){
		cout << "服务端连接失败，请验证服务端IP地址！" << endl;
		exit(-1);
	}

	//发送请求数据给服务端
	m_socket.sendMsg(outData, dataLen);

	//等待接收服务端的应答
	char *inData;
	m_socket.recvMsg(&inData, dataLen);

	//解码
	factory = new RespondFactory();
	pCodec = factory->createCodec();
	RespondMsg *pMsg = (RespondMsg *)pCodec->msgDecode(inData, dataLen);
	
	// 用于存储写入共享内存的秘钥信息
	NodeSHMInfo node;

	switch (pMsg->rv)
	{
	case RespondMsg::Success:
		std::cout << "秘钥协商成功" << std::endl;
		//将服务端的r2和客户端的r1拼接生成秘钥
		unsigned char md1[SHA_DIGEST_LENGTH];
		memset(md1, 0x00, sizeof(md1));
		char seckey[SHA_DIGEST_LENGTH*2+1];
		memset(seckey, 0x00, sizeof(seckey));
		
		char buf[1024];
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "%s%s", req.r1, pMsg->r2);
		// SHA1((unsigned char *)buf, strlen((char *)buf), md1);
		EVP_Digest(buf,strlen((char *)buf),md1,NULL,EVP_sha1(),NULL);
		for(int i=0; i<SHA_DIGEST_LENGTH; i++)
		{ 
			sprintf(&seckey[i*2], "%02x", md1[i]);
		}
		cout << "秘钥1: " << seckey << endl;
		

		//给秘钥结构体赋值
		memset(&node, 0x00, sizeof(NodeSHMInfo));
		node.status = 0;
		strcpy(node.seckey, seckey);
		strcpy(node.clientID, m_info.clinetID);
		strcpy(node.serverID, m_info.serverID);
		node.seckeyID = pMsg->seckeyid;

		//将秘钥信息写入共享内存
		pthread_rwlock_wrlock(&shmem_wlock);
		m_shm->shmWrite(&node);
		pthread_rwlock_unlock(&shmem_wlock);

		/* code */
		break;
	case RespondMsg::ClientNoExist:
		std::cout << "客户端为注册，请注册后再申请！" << std::endl;
		break;
	case RespondMsg::ServerIDError:
		std::cout << "服务端ID错误，请重启后输入正确服务端ID！" << std::endl;
		break;
	case RespondMsg::DataPassErr:
		std::cout << "秘钥传输过程中出错，请重新申请！" << std::endl;
		break;
	default:
		break;
	}
	
	//关闭网络连接
	m_socket.disConnect();
	//释放资源
	delete factory;
	delete pCodec;

	return 0;
}

// char randBuf[64]; , 参数 64, randBuf
void ClientOperation::getRandString(int len, char * randBuf)
{
	int flag = -1;
	// 设置随机种子
	srand(time(NULL));
	// 随机字符串: A-Z, a-z, 0-9, 特殊字符(!@#$%^&*()_+=)
	char chars[] = "!@#$%^&*()_+=";
	for (int i = 0; i < len-1; ++i)
	{
		flag = rand() % 4;
		switch (flag)
		{
		case 0:
			randBuf[i] = rand() % 26 + 'A';
			break;
		case 1:
			randBuf[i] = rand() % 26 + 'a';
			break;   
		case 2:
			randBuf[i] = rand() % 10 + '0';
			break;
		case 3:
			randBuf[i] = chars[rand() % strlen(chars)];
			break;
		default:
			break;
		}
	}
	randBuf[len - 1] = '\0';
}
