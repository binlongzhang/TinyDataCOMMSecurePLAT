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
