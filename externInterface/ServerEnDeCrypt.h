/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-28 06:22:51
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 03:46:50
 */
#pragma once
#include <openssl/evp.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "NodeSHMInfo.h"

class ServerEnDeCrypt
{
private:
    char serverID[12]; // 服务器端编号
    int shmID;
    void *m_shmAddr;
    pthread_rwlock_t rlock;
    bool updateKeyAndIV(unsigned char *key, unsigned char *iv, int size, char *clientID);

public:
    const EVP_CIPHER *(*crpty_cipher)();

    ServerEnDeCrypt(int shmkey, char *severID);
    ~ServerEnDeCrypt();
    int getEncryptLen(int plaintext_len) { return 16 * (plaintext_len / 16 + 1); };
    int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *result, char *clientID);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *result, char *clientID);
};
