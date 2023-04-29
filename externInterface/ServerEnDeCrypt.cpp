/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-28 06:22:37
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 03:44:57
 */
#include "ServerEnDeCrypt.h"
#include <string.h>
#include <cstdlib>
#include <iostream>


ServerEnDeCrypt::ServerEnDeCrypt(int shmkey, char *serverID)
{
    this->shmID = shmget(shmkey, 0, 0);
    this->crpty_cipher = EVP_aes_128_cbc;
    memcpy(this->serverID, serverID, 12);
    m_shmAddr = nullptr;
    m_shmAddr = shmat(this->shmID, NULL, 0);

    // 初始化读锁
    pthread_rwlockattr_t rw_att;
    pthread_rwlockattr_init(&rw_att);
    pthread_rwlockattr_setkind_np(&rw_att, PTHREAD_RWLOCK_PREFER_WRITER_NP);
    pthread_rwlockattr_setpshared(&rw_att, PTHREAD_PROCESS_SHARED);
    pthread_rwlock_init(&rlock, &rw_att);
    pthread_rwlockattr_destroy(&rw_att);

    if (m_shmAddr == nullptr)
    {
        //	错误日志
        std::cout << "共享内存获取失败，秘钥未写入，请连接服务端并生成秘钥" << std::endl;
        exit(0);
    }
}

ServerEnDeCrypt::~ServerEnDeCrypt()
{
    pthread_rwlock_destroy(&rlock);
    if (shmdt(m_shmAddr) != 0)
    {
        std::cout << "共享内存脱离失败！" << std::endl;
    }
}

// 加锁，拷贝key和iv
bool ServerEnDeCrypt::updateKeyAndIV(unsigned char *key, unsigned char *iv, int size, char *clientID)
{
    //	获取节点数
    int maxNode;
    pthread_rwlock_rdlock(&rlock);
    memcpy(&maxNode, m_shmAddr, sizeof(int));
    std::cout << "maxNode:" << maxNode << std::endl;

    NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)m_shmAddr + sizeof(int));

    int i = 0;
    for (i = 0; i < maxNode; i++)
    {
        if (strcmp(clientID, pNode[i].clientID) == 0 &&
            strcmp(serverID, pNode[i].serverID) == 0)
        {
            break;
        }
    }

    //	没找到对应的信息
    if (i == maxNode)
    {
        std::cout << "not found" << std::endl;
        return false;
    }
    else
    {
        memcpy(key, pNode[i].seckey, sizeof(unsigned char) * 16);
        memcpy(iv, (pNode[i].seckey) + 20, sizeof(unsigned char) * 16);
    }

    pthread_rwlock_unlock(&rlock);
    return true;
}

/// @brief 数据加密
/// @param plaintext: 加密数据的头指针
/// @param plaintext_len: 加密数据的长度
/// @param ciphertext: 接受数据的长度，容量大小应该大于通过 int getEncryptLen(int plaintext_len)获得；
/// @param client: 客户端ID
/// @return 返回密文长度
int ServerEnDeCrypt::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *result, char *clientID)
{
    int resultLen;
    unsigned char key[17];
    unsigned char iv[17];
    memset(key, 0, 17);
    memset(iv, 0, 17);
    updateKeyAndIV(key, iv, 16, clientID);
    EVP_CIPHER_CTX *ctx;
    int len;

    /* Create and initialize the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
    {
        return -1;
    }

    /* Initialize the encryption operation */
    if (EVP_EncryptInit(ctx, crpty_cipher(), key, iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    printf("%d\n", EVP_CIPHER_get_block_size(crpty_cipher()));

    /* Encrypt the plaintext */
    if (EVP_EncryptUpdate(ctx, result, &len, plaintext, plaintext_len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    resultLen = len;

    /* Finalize the encryption */
    if (EVP_EncryptFinal(ctx, result + resultLen, &len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    resultLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return resultLen;
}

/// @brief 解密数据
/// @param ciphertext:  密文
/// @param ciphertext_len： 密文长度
/// @param result： 结果
/// @param clientID： 客户端ID
/// @return 返回解密后文本长度
int ServerEnDeCrypt::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *result, char *clientID)
{
    int resultLen;
    unsigned char key[17];
    unsigned char iv[17];
    memset(key, 0, 17);
    memset(iv, 0, 17);
    updateKeyAndIV(key, iv, 16, clientID);
    EVP_CIPHER_CTX *ctx;
    int len;

    /* Create and initialize the context */
    if (!(ctx = EVP_CIPHER_CTX_new()))
        return -1;

    /* Initialize the decryption operation */
    if (EVP_DecryptInit(ctx, crpty_cipher(), key, iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    /* Decrypt the ciphertext */
    if (EVP_DecryptUpdate(ctx, result, &len, ciphertext, ciphertext_len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    resultLen = len;

    /* Finalize the decryption */
    if (EVP_DecryptFinal(ctx, result + resultLen, &len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    resultLen += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return resultLen;
}