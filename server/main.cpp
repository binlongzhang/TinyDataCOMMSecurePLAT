/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-20 12:21:32
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 09:07:45
 */
#include <cstdio>
#include "ServerOperation.h"
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
using namespace std;

void createDeamon();

int main(int argc, char *argv[])
{

	if (argc < 8)
	{
		printf("按照如下格式运行: ./%s serverID maxServerNode port databaseIP databaseUser databasePWD databasedbName \n", basename(argv[0]));
		printf("e.g. ./%s 0001 20 9898 localhost  root password dbName\n", basename(argv[0]));
		return -1;
	}
	std::string dbIP(argv[4]);
	std::string dbUser(argv[5]);
	std::string dbPWD(argv[6]);
	std::string dbName(argv[7]);

	createDeamon();
	// 启动服务器
	ServerInfo info;
	strcpy(info.serverID, argv[1]);
	info.maxnode = atoi(argv[2]);
	info.sPort = atoi(argv[3]);
	info.shmkey = ftok("/home", 'w');

	ServerOperation op(&info, dbIP, dbUser, dbPWD, dbName);
	op.startWork();

	cout << "good bye..." << endl;

	return 0;
}

void createDeamon()
{
	// 1. 创建子进程, 杀死父进程
	pid_t pid = fork();
	// 父进程
	if (pid > 0)
	{
		exit(0);
	}
	// 子进程
	// 2. 子进程提升为会话
	setsid();
	// 3. 修改工作目录
	chdir("/home");
	// 4. 设置掩码
	umask(0022);

	// 5. 重定向文件描述符 - /dev/null-->黑洞设备
	int devfd = open("/dev/null", O_RDWR);
	dup2(devfd, STDIN_FILENO);
	// dup2(devfd, STDOUT_FILENO); // 调试的时候需要看输出
	dup2(devfd, STDERR_FILENO);
}