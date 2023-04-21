#define _CRT_SECURE_NO_WARNINGS
#include <openssl/md5.h>		// md5 ͷ�ļ�
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void getMD5(const char* str, char* result)
{
	MD5_CTX ctx;
	// 初始化
	MD5_Init(&ctx);
	// 更新
	MD5_Update(&ctx, str, strlen(str));
	// 存放加密数据
	unsigned char md[16] = { 0 };
	MD5_Final(md, &ctx);
	for (int i = 0; i < 16; ++i) 
	{
		sprintf(&result[i * 2], "%02x", md[i]);
	}
}

int main()
{
	char result[33] = { 0 };
	getMD5("hello, md5", result);
	printf("md5 value: %s\n", result);

	return 0;
}