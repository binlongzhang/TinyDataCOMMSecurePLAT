#ifndef BASEASN1_H
#define BASEASN1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyLog.h"

// #define MyDER_NoErr 0
// 自定义基础数据类型
typedef int              My_INT;
typedef unsigned char    My_UINT8;
typedef unsigned short   My_UINT16;
typedef unsigned long    My_UINT32;
typedef signed long      My_SINT32;
typedef unsigned char    ITASN1_BOOLEAN;

// 数据节点结构体(类)
typedef struct My_ANYBUF_
{
    My_UINT8 	  *pData;
    My_UINT32     dataLen;

    My_UINT32     unusedBits;	/* for bit string */
    My_UINT32     memoryType;
    My_UINT32     dataType;
    struct My_ANYBUF_ *next;    /* for sequence and set */
    struct My_ANYBUF_ *prev;
}My_ANYBUF;

// 自定义复合数据类型
typedef My_ANYBUF    ITASN1_INTEGER;
typedef My_ANYBUF    ITASN1_OCTETSTRING;
typedef My_ANYBUF    ITASN1_BITSTRING;
typedef My_ANYBUF    ITASN1_PRINTABLESTRING;
/*begin of bmpstring*/
typedef My_ANYBUF	 ITASN1_BMPSTRING;
/*end of bmpstring*/
typedef My_ANYBUF    ITASN1_ENUMERATED;
typedef My_ANYBUF    ITASN1_IA5STRING;
typedef My_ANYBUF    ITASN1_SEQUENCE;
typedef My_ANYBUF    ITASN1_SET;

class BaseASN1
{
public:
    // 标记内存类型
    enum MemoryType{StaticMemory=1, MallocMemory=2};
    // 错误描述
    enum DerErrType{
        NoErr=0,
        MemoryErr=200,
        LengthErr,
        LengthNotEqual,
        DataRangeErr,
        InvalidTag
    };
    // 整形数的每一位代表不同的数据类型0-31, 即: tag的值对应的数据类型
    enum DerTag{
        My_DER_ID_RESERVED,
        My_DER_ID_BOOLEAN,
        My_DER_ID_INTEGER,
        My_DER_ID_BITSTRING,
        My_DER_ID_OCTETSTRING,
        My_DER_ID_NULL,
        My_DER_ID_OBJECT_IDENTIFIER,
        My_DER_ID_OBJECT_DESCRIPTOR,
        My_DER_ID_EXTERNAL,
        My_DER_ID_REAL,
        My_DER_ID_ENUMERATED,
        My_DER_ID_EMBEDDED_PDV,
        My_DER_ID_STRING_UTF8,
        My_DER_ID_13,
        My_DER_ID_14,
        My_DER_ID_15,
        My_DER_ID_SEQUENCE,
        My_DER_ID_SET,
        My_DER_ID_STRING_NUMERIC,
        My_DER_ID_STRING_PRINTABLE,
        My_DER_ID_STRING_T61,
        My_DER_ID_STRING_VIDEOTEX,
        My_DER_ID_STRING_IA5,
        My_DER_ID_TIME_UTC,
        My_DER_ID_TIME_GENERALIZED,
        My_DER_ID_STRING_GRAPHIC,
        My_DER_ID_STRING_ISO646,
        My_DER_ID_STRING_GENERAL,
        My_DER_ID_STRING_UNIVERSAL,
        My_DER_ID_29,
        My_DER_ID_STRING_BMP
    };

    // 构造函数
    BaseASN1();

    My_INT DER_ItAsn1_WriteInteger(My_UINT32 integer, ITASN1_INTEGER **ppDerInteger);

    My_INT DER_ItAsn1_ReadInteger(ITASN1_INTEGER *pDerInteger, My_UINT32 *pInteger);

    My_INT DER_ItAsn1_WriteBitString(ITASN1_BITSTRING *pBitString, ITASN1_BITSTRING **ppDerBitString);

    My_INT DER_ItAsn1_ReadBitString(ITASN1_BITSTRING *pDerBitString, ITASN1_BITSTRING **ppBitString);

    My_INT DER_ItAsn1_WritePrintableString(ITASN1_PRINTABLESTRING *pPrintString, ITASN1_PRINTABLESTRING **ppDerPrintString);

    My_INT DER_ItAsn1_ReadPrintableString(ITASN1_PRINTABLESTRING *pDerPrintString, ITASN1_PRINTABLESTRING **ppPrintString);

    My_INT DER_ItAsn1_WriteSequence(ITASN1_SEQUENCE *pSequence, My_ANYBUF **ppDerSequence);

    My_INT DER_ItAsn1_ReadSequence(My_ANYBUF *pDerSequence, ITASN1_SEQUENCE **ppSequence);

    My_INT DER_ItAsn1_WriteNull(My_ANYBUF ** ppDerNull);

    My_INT DER_ItAsn1_ReadNull(My_ANYBUF * ppDerNull, My_UINT8 * pInt);

    // 释放一个序列(链表), pAnyBuf为链表的头结点
    My_INT DER_My_FreeQueue(My_ANYBUF *pAnyBuf);

    // 创建My_ANYBUF, 将strOrigin写入创建的My_ANYBUF内存中, 通过pOriginBuf将内存地址传出
    My_INT DER_My_String_To_AnyBuf(My_ANYBUF **pOriginBuf, unsigned char * strOrigin, int strOriginLen);

    int WriteNullSequence(My_ANYBUF **pOutData);

    // 同 EncodeChar 函数
    int EncodeUnsignedChar(unsigned char *pData, int dataLen, My_ANYBUF **outBuf);

    int DecodeUnsignedChar(My_ANYBUF *inBuf, unsigned char **Data, int *pDataLen);

    // pData编码为My_ANYBUF(有malloc动作), 将新的My_ANYBUF节点地址赋值给outBuf
    int EncodeChar(char *pData, int dataLen, My_ANYBUF **outBuf);

    // 解析节点inBuf中的字符串数据, 通过第二个参数Data指针传出
    int DecodeChar(My_ANYBUF *inBuf, char **Data, int *pDataLen);

private:
    My_INT DER_ItAsn1_Low_GetTagInfo(
            My_UINT8 **ppDerData,
            My_UINT32 **ppTagValue,
            My_UINT32 **ppTagSize);
    My_UINT32 DER_ItAsn1_Low_Count_LengthOfSize(My_UINT32 iLength);
    My_INT DER_ItAsn1_GetLengthInfo(
            My_ANYBUF *pDerData,
            int *pLengthValue,
            int *pLengthSize);
    My_INT DER_ItAsn1_Low_GetLengthInfo(
            My_UINT8 **ppDerData,
            My_UINT32 **ppLengthValue,
            My_UINT32 **ppLengthSize);
    My_INT DER_ItAsn1_Low_IntToChar(
            My_UINT32 integer,
            My_UINT8 **ppData,
            My_UINT32 **ppLength);
    My_INT DER_ItAsn1_Low_CharToInt(
            My_UINT8 *aData,
            My_UINT32 lLength,
            My_UINT32 **ppInteger);
    My_INT DER_ItAsn1_Low_WriteTagAndLength(
            My_ANYBUF *pAnyIn,
            My_UINT8 cTag,
            My_ANYBUF **ppAnyOut,
            My_UINT8 **ppUint8Value);
    My_INT DER_ItAsn1_Low_ReadTagAndLength(
            My_ANYBUF *pAnyIn,
            My_UINT8 **ppUint8Data,
            My_ANYBUF **ppAnyOut,
            My_UINT8 **ppUint8Value);
    My_INT DER_ItAsn1_WriteCharString(
            My_ANYBUF *pCharString,
            My_ANYBUF **ppDerCharString);
    My_INT DER_ItAsn1_ReadCharString(
            My_ANYBUF *pDerCharString,
            My_ANYBUF **ppCharString);
    My_INT DER_ItAsn1_WriteBmpString(
            ITASN1_BMPSTRING *pBmpString,
            ITASN1_BMPSTRING **ppDerBmpString);
    My_INT DER_ItAsn1_ReadBmpString(
            ITASN1_BMPSTRING *pDerBmpString,
            ITASN1_BMPSTRING **ppBmpString);
    void DER_DI_FreeAnybuf(My_ANYBUF  * pAnyBuf);
    // 给一个My_ANYBUF类型空节点指针分配存储空间
    int DER_CREATE_LOW_My_ANYBUF(My_ANYBUF *&point);
    // 内联函数
    inline void DER_My_Free(void *memblock)
    {
        if(memblock)
        {
            free(memblock);
            memblock = NULL;
        }
    }
    // 计算数据类型对应的tag
    inline My_UINT32 DER_ITASN1_LOW_IDENTIFIER(My_UINT8 &cTag)
    {
        return cTag & My_DER_SHORT_ID_MASK;
    }
    // 在堆上创建一个指定大小的数组, 使用指针的引用 == 使用指针的指针
    inline int DER_ITASN1_LOW_CREATEUINT8(My_UINT8* &point, My_UINT32 size)
    {
        point = (My_UINT8*)malloc(size);
        if (point==NULL)
        {
            return 7002;
        }
        //memset(point,0,size)
        return 0;
    }
    // 在堆上创建一个 My_UINT32 大小的内存
    // 原来的宏参数是指针, 所以此次应该使用指针的引用
    inline int DER_ITASN1_LOW_CREATEUINT32(My_UINT32* &point)
    {
        point = (My_UINT32*)malloc(sizeof(My_UINT32));
        if (point==NULL)
            return 7002;
        memset(point, 0, sizeof(My_UINT32));
        return 0;
    }
    // 通过检测iResult的值, 返回错误类型
    inline DerErrType DER_ITASN1_LOW_CHECKERR(My_INT iResult, DerErrType iErrNumber)
    {
        if (iResult == iErrNumber)
            return iErrNumber;
        return NoErr;
    }
    // 释放节点内存
    inline void DER_DACERT_LOW_FREE_ANYBUF(My_ANYBUF *point)
    {
        DER_My_Free(point->pData);
        DER_My_Free(point);
    }

private:
    MyLog m_log;       // log类

	const My_UINT8 TRUE = 1;
	const My_UINT8 FALSE = 0;
	/* 标记当前数据是原始数据还是编码数据 */
	const My_UINT32 My_DER_CONSTRUCTED = 0x20;
	const My_UINT32 My_DER_PRIMITIVE = 0x00;

	/* The encodings for the universal types */
	const My_UINT32 My_DER_UNIVERSAL = 0x00;
	const My_UINT32 My_DER_APPLICATION = 0x40;
	const My_UINT32 My_DER_CONTEXT_SPECIFIC = 0x80;
	const My_UINT32 My_DER_PRIVATE = 0xC0;

	const My_UINT32 My_DER_RESERVED = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_RESERVED);
	const My_UINT32 My_DER_BOOLEAN = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_BOOLEAN);
	const My_UINT32 My_DER_INTEGER = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_INTEGER);
	const My_UINT32 My_DER_BITSTRING = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_BITSTRING);
	const My_UINT32 My_DER_OCTETSTRING = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_OCTETSTRING);
	const My_UINT32 My_DER_NULL = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_NULL);
	const My_UINT32 My_DER_OBJECT_IDENTIFIER = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_OBJECT_IDENTIFIER);
	const My_UINT32 My_DER_OBJECT_DESCRIPTOR = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_OBJECT_DESCRIPTOR);
	const My_UINT32 My_DER_EXTERNAL = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_EXTERNAL);
	const My_UINT32 My_DER_REAL = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_REAL);
	const My_UINT32 My_DER_ENUMERATED = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_ENUMERATED);
	const My_UINT32 My_DER_EMBEDDED_PDV = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_EMBEDDED_PDV);
	const My_UINT32 My_DER_STRING_UTF8 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_UTF8);
	const My_UINT32 My_DER_13 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_13);
	const My_UINT32 My_DER_14 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_14);
	const My_UINT32 My_DER_15 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_15);
	const My_UINT32 My_DER_SEQUENCE = (My_DER_UNIVERSAL | My_DER_CONSTRUCTED | My_DER_ID_SEQUENCE);
	const My_UINT32 My_DER_SET = (My_DER_UNIVERSAL | My_DER_CONSTRUCTED | My_DER_ID_SET);
	const My_UINT32 My_DER_STRING_NUMERIC = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_NUMERIC);
	const My_UINT32 My_DER_STRING_PRINTABLE = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_PRINTABLE);
	const My_UINT32 My_DER_STRING_T61 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_T61);
	const My_UINT32 My_DER_STRING_VIDEOTEX = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_VIDEOTEX);
	const My_UINT32 My_DER_STRING_IA5 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_IA5);
	const My_UINT32 My_DER_TIME_UTC = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_TIME_UTC);
	const My_UINT32 My_DER_TIME_GENERALIZED = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_TIME_GENERALIZED);
	const My_UINT32 My_DER_STRING_GRAPHIC = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_GRAPHIC);
	const My_UINT32 My_DER_STRING_ISO646 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_ISO646);
	const My_UINT32 My_DER_STRING_GENERAL = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_GENERAL);
	const My_UINT32 My_DER_STRING_UNIVERSAL = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_UNIVERSAL);
	const My_UINT32 My_DER_29 = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_29);
	const My_UINT32 My_DER_STRING_BMP = (My_DER_UNIVERSAL | My_DER_PRIMITIVE | My_DER_ID_STRING_BMP);

	/* Masks to extract information from a tag number */
	const My_UINT32 My_DER_CLASS_MASK = 0xC0;
	const My_UINT32 My_DER_CONSTRUCTED_MASK = 0x20;
	const My_UINT32 My_DER_SHORT_ID_MASK = 0x1F;
	const My_UINT32 My_DER_FIRST_NOT_ID_MASK = 0x7F;     //xia
	const My_UINT32 My_DER_FIRST_YES_ID_MASK = 0x80;     //xia
	const My_UINT32 My_DER_ALL_YES_ID_MASK = 0xFF;     //xia
															   /* The maximum size for the short tag number encoding, and the magic value
															   which indicates that a long encoding of the number is being used */
	const My_UINT32 ITASN1_MAX_SHORT_BER_ID = 30;
	const My_UINT32 ITASN1_LONG_BER_ID = 0x1F;

};

#endif // BASEASN1_H
