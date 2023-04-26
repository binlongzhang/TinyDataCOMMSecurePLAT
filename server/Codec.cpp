/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-20 12:21:32
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 21:36:25
 */
#include "Codec.h"

Codec::Codec()
{
}

Codec::~Codec()
{
}
// 仅仅提供接口，具体由子类实现
int Codec::msgEncode(char ** outData, int & len)
{
	return 0;
}

// 仅仅提供接口，具体由子类实现
void * Codec::msgDecode(char * inData, int inLen)
{
	return NULL;
}
