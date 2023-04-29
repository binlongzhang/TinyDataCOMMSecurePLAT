<!--
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-29 04:49:25
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-29 07:39:03
-->
# 数据安全传输基础平台-Demo
> 基于OpenSSL实现数据加密传输基础平台

# 基本功能
> 为客户端和服务端运行的各类进程提供数据加密传输服务

1. 服务器运行启动**秘钥协商服务端**，并连接用户信息数据库
2. 客户端运行**秘钥协商客户端**，并向服务端发送秘钥协商请求
3. 服务端经过网络传输数据校验，并验证用户身份信息之后，回应客户端并生成协商秘钥
4. 秘钥协商完成后，客户端服务器之间通过**外联接口**实现安全传输的第三方进程都会通过刚刚协商的秘钥实现加密传输

# 项目结构
## server
*秘钥协商服务端*
- makefile 生成服务端可执行程序 serverMain
- stopServer.sh 用于停止服务端程序
- DB 主要存放服务端正常运行时所需的数据库表相关信息
  - 以 mysql 为例
  - SSL_proj.sql 为生成简易数据库表的脚本
  - 剩余的为简单测试数据库连接demo，对该项目无实际意义

## client
*秘钥协商服务端*
- 目前只提供命令操作,且只支持基础功能
- makefile 生成客户端可执行程序 clientMain

## externInterface
外联接口，给通信的第三方应用提供加密接口
- 接口类
  - 客户端接口类: ClientEnDeCrypt.h ClientEnDeCrypt.cpp
  - 服务端接口类: ServerEnDeCrypt.h ServerEnDeCrypt.cpp
- 调用示例
  - 客户端加密解密调用示例
    - ClientDemo.cpp
    - 通过调用 make clientDemo 生成客户端可执行程序示例 
  - 服务端加密解密调用示例
    - ServerDemo.cpp
    - 通过调用 make serverDemo 生成服务端可执行程序示例 