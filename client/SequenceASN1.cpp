#include "SequenceASN1.h"

SequenceASN1::SequenceASN1() : BaseASN1()
{
}

int SequenceASN1::writeHeadNode(int iValue)
{
	int ret = DER_ItAsn1_WriteInteger(iValue, &m_header);
	if (ret != 0)
	{
		return ret;
	}
	m_next = m_header;
	return 0;
}

int SequenceASN1::writeHeadNode(char * sValue, int len)
{
	DER_My_String_To_AnyBuf(&m_temp, (unsigned char*)sValue, len);
	int ret = DER_ItAsn1_WritePrintableString(m_temp, &m_header);
	if (ret != 0)
	{
		return ret;
	}
	m_next = m_header;
	DER_My_FreeQueue(m_temp);

	return 0;
}

int SequenceASN1::writeNextNode(int iValue)
{
	int ret = DER_ItAsn1_WriteInteger(iValue, &m_next->next);
	if (ret != 0)
	{
		return ret;
	}
	// 后移next指针
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::writeNextNode(char * sValue, int len)
{
	int ret = EncodeChar(sValue, len, &m_next->next);
	if (ret != 0)
	{
		return ret;
	}
	m_next = m_next->next;

	return 0;
}

/// @brief 从序列中读出一个整数
/// @param sValue 输出参数，引用格式
/// @return 0为成功，其他为失败
int SequenceASN1::readHeadNode(int & iValue)
{
	int ret = DER_ItAsn1_ReadInteger(m_header, (My_UINT32*)&iValue);
	if (ret != 0)
	{
		return ret;
	}
	// 后移指针
	m_next = m_header->next;

	return 0;
}

/// @brief 从序列中读出一个char*表示的字符串，'\0'标识结尾
int SequenceASN1::readHeadNode(char * sValue)
{
	int ret = DER_ItAsn1_ReadPrintableString(m_header, &m_temp);
	if (ret != 0)
	{
		return ret;
	}
	memcpy(sValue, m_temp->pData, m_temp->dataLen);
	DER_My_FreeQueue(m_temp);
	// 后移next指针
	m_next = m_header->next;
	return 0;
}

int SequenceASN1::readNextNode(int & iValue)
{
	int ret = DER_ItAsn1_ReadInteger(m_next, (My_UINT32*)&iValue);
	if (ret != 0)
	{
		return ret;
	}
	// 后移指针
	m_next = m_next->next;
	return 0;
}

int SequenceASN1::readNextNode(char * sValue)
{
	int ret = DER_ItAsn1_ReadPrintableString(m_next, &m_temp);
	if (ret != 0)
	{
		return ret;
	}
	memcpy(sValue, m_temp->pData, m_temp->dataLen);
	DER_My_FreeQueue(m_temp);
	// 后移指针
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::packSequence(char ** outData, int & outLen)
{
	DER_ItAsn1_WriteSequence(m_header, &m_temp);
	// 将打包数据传出
	*outData = (char*)m_temp->pData;
	outLen = m_temp->dataLen;
	// 释放当前序列
	DER_My_FreeQueue(m_header);

	return 0;
}

//解包前应该要置m_header为空，并且m_next位置位置，最好谨慎使用该函数
int SequenceASN1::unpackSequence(char * inData, int inLen)
{
	// char* -> My_anybuf
	DER_My_String_To_AnyBuf(&m_temp, (unsigned char*)inData, inLen);
	DER_ItAsn1_ReadSequence(m_temp, &m_header);
	DER_My_FreeQueue(m_temp);

	return 0;
}

void SequenceASN1::freeSequence(My_ANYBUF * node){
	if(node!=NULL){
		DER_My_FreeQueue(node);
	}
}
