/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-26 12:35:55
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 03:43:57
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <iostream>
#include "ClientEnDeCrypt.h"


int main(int argc, char *argv[])
{
    // 共享内存ID
	int shmKey = 0x12345678;
    // 待加密数据
    unsigned char plaintext[] = "Hello, world! Here is an demo of Client!";
    int plaintext_len = strlen((char *)plaintext);
    printf("strLen : %d\n",plaintext_len);

    ClientEnDeCrypt clientCrypt(shmKey);


    // 加密
    unsigned char * ciphertext = new unsigned char[clientCrypt.getEncryptLen(plaintext_len)];
    memset(ciphertext,0,clientCrypt.getEncryptLen(plaintext_len));
    int ciphertext_len = clientCrypt.encrypt(plaintext,plaintext_len,ciphertext);
    printf("Ciphertext is:\n");
    BIO_dump_fp(stdout, ciphertext, ciphertext_len);


    //解密
    unsigned char * decryptedtext = new unsigned char[ciphertext_len];
    memset(decryptedtext,0,ciphertext_len);
    int decryptedtext_len = clientCrypt.decrypt(ciphertext, ciphertext_len,decryptedtext);
    printf("\nDecrypted text is:\n");
    printf("%s\n", (char*)decryptedtext);


    return 0;
}
