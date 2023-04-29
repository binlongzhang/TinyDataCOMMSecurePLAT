/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-28 04:04:17
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 04:13:02
 */
#pragma once
#include <pthread.h>
#include <openssl/evp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "NodeSHMInfo.h"

class ClientEnDeCrypt
{
private:
    int shmID;
    NodeSHMInfo *keyinfo;
    void * m_shmAddr;
    bool updateKeyAndIV(unsigned char* key,unsigned char* iv,int size);
    pthread_rwlock_t rlock;

public:
    const EVP_CIPHER* (*crpty_cipher)();

    ClientEnDeCrypt(int shmkey);
    ~ClientEnDeCrypt();
    int getEncryptLen(int plaintext_len){ return 16*(plaintext_len/16 + 1);};
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *result);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *result);
};
