/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-19 12:24:23
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 08:04:52
 */
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "RequestCodec.h"
#include "RespondCodec.h"
#include "RequestFactory.h"
#include "RespondFactory.h"
#include "ClientOperation.h"
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;
int usage();

int main(int argc, char*argv[]){
    
    if(argc < 5){
        printf("按照如下格式运行: ./%s serverIP server_port serverID clientID \n",basename(argv[0]));
		printf("e.g. ./%s 127.0.0.1 9898 0001 1111 \n",basename(argv[0]));
		return -1;
    }
	
	ClientInfo info;
	memset(&info, 0x00, sizeof(ClientInfo));
	// strcpy(info.serverIP, "127.0.0.1");
	// info.serverPort = 9898;
	// strcpy(info.serverID, "0001");
	// strcpy(info.clinetID, "1111");

	strcpy(info.serverIP, argv[1]);
	info.serverPort = atoi(argv[2]);
	strcpy(info.serverID, argv[3]);
	strcpy(info.clinetID, argv[4]);

	info.maxNode = 1;
	info.shmKey = 0x12345678;
	ClientOperation client(&info);

	//enum CmdType{NewOrUpdate=1, Check, Revoke, View};
	int nSel;
	while (1)
	{
		nSel = usage();
		switch(nSel)
		{
		case RequestCodec::NewOrUpdate:
			client.secKeyAgree();
			break;

		case RequestCodec::Check:
			client.secKeyCheck();
			break;
		case RequestCodec::Revoke:
			client.secKeyRevoke();
			break;
		case RequestCodec::View:
			client.secKeyView();
			break;
		case 0:
		     exit(0);

		default:
			break;
		}
	}	
}

int usage()
{
	int nSel = -1;
	printf("\n  /*************************************************************/");
	printf("\n  /*     1.密钥协商                                            */");
	printf("\n  /*     2.密钥校验                                          */");
	printf("\n  /*     3.密钥注销                                            */");
	printf("\n  /*     4.密钥查看                                           */");
	printf("\n  /*     0.退出系统                                        */");
	printf("\n  /*************************************************************/");
	printf("\n\n  选择:");

	scanf("%d", &nSel);
	while (getchar() != '\n');

	return nSel;
}
