#include "SecKeyShm.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;

// int main(int argc, char *argv[])
// {
// 	char clientID[16] = {0};
// 	char serverID[16] = {0};
// 	strcpy(clientID, argv[1]);
// 	strcpy(serverID, argv[2]);

// 	// SecKeyShm shm(0x1234, 10);
// 	SecKeyShm shm(0x1234);

// 	NodeSHMInfo info;
// 	memset(&info, 0x00, sizeof(NodeSHMInfo));
// 	info.status = 199;
// 	info.seckeyID = 11;
// 	strcpy(info.clientID, clientID);
// 	strcpy(info.serverID, serverID);
// 	strcpy(info.seckey, "0001");
// 	shm.shmWrite(&info);

// 	cout << "----------------" << endl;
// 	SecKeyShm shm1(0x1234);
// 	memset(&info, 0x00, sizeof(NodeSHMInfo));
// 	shm1.shmRead(clientID, serverID, &info);

// 	cout << "status:" << info.status << endl;
// 	cout << "seckeyID:" << info.seckeyID << endl;
// 	cout << "clientID:" << info.clientID << endl;
// 	cout << "serverID:" << info.serverID << endl;
// 	cout << "seckey:" << info.seckey << endl;

// 	cout << "----------------" << endl;


// 	shm1.printShm();

// 	return 0;
// }
