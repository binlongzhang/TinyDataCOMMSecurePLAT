/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-28 04:04:05
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 04:15:26
 */

#include "ClientEnDeCrypt.h"
#include <string.h>
#include <cstdlib>
#include <iostream>
/// @brief 用于客户端调用秘钥进行加密操作
/// @param shmkey: 秘钥协商客户端设置的共享内存的key值
ClientEnDeCrypt::ClientEnDeCrypt(int shmkey)
{
    this->shmID = shmget(shmkey, 0, 0);
    this->crpty_cipher = EVP_aes_128_cbc;
    m_shmAddr = nullptr;
    m_shmAddr = shmat(this->shmID, NULL, 0);
    keyinfo = (NodeSHMInfo *)((char *)m_shmAddr + sizeof(int));

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

ClientEnDeCrypt::~ClientEnDeCrypt()
{
    pthread_rwlock_destroy(&rlock);
    if (shmdt(m_shmAddr) != 0)
    {
        std::cout << "共享内存脱离失败！" << std::endl;
    }
}

// 加锁，拷贝key和iv
bool ClientEnDeCrypt::updateKeyAndIV(unsigned char *key, unsigned char *iv, int size)
{
    pthread_rwlock_rdlock(&rlock);
    memcpy(key, keyinfo->seckey, sizeof(unsigned char) * 16);
    memcpy(iv, (keyinfo->seckey) + 20, sizeof(unsigned char) * 16);
    pthread_rwlock_unlock(&rlock);
    return 0;
}

/// @brief 数据加密
/// @param plaintext: 加密数据的头指针
/// @param plaintext_len: 加密数据的长度
/// @param ciphertext: 接受数据的长度，容量大小通过 int getEncryptLen(int plaintext_len)获得
/// @return 返回密文长度
int ClientEnDeCrypt::encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *result)
{
    int resultLen;
    unsigned char key[17];
    unsigned char iv[17];
    memset(key, 0, 17);
    memset(iv, 0, 17);
    updateKeyAndIV(key, iv, 16);
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

/// @brief 解密数据，线程安全
/// @param ciphertext   密文
/// @param ciphertext_len   密文长度
/// @param result   解密结果
/// @return 返回解密后文本长度
int ClientEnDeCrypt::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *result)
{
    int resultLen;
    unsigned char key[17];
    unsigned char iv[17];
    memset(key, 0, 17);
    memset(iv, 0, 17);
    updateKeyAndIV(key, iv, 16);
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