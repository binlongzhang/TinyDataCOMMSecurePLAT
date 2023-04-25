/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-25 04:49:45
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 06:29:22
 */
#include <iostream>
#include "CMysqlOP.h"
#include "SecKeyShm.h"
#include <string.h>

int main(){
    CMysqlOP sqlObj;
    if(!sqlObj.connectDB("localhost","zhangbinglong","1094859023","ssl_proj")){
        std::cout<< "conenet error"<<std::endl;
        return -1;
    }

    std::cout<< sqlObj.getKeyID() <<std::endl;
    if(!sqlObj.updataKeyID(2)){
        std::cout<< "Modify failed"<<std::endl;
    }

    std::cout<< "success modify to :"<<sqlObj.getKeyID() <<std::endl;
    NodeSHMInfo info;
    info.status = 0;
    info.seckeyID = 2;
    strcpy(info.clientID,"1111");
    strcpy(info.serverID,"1");
    strcpy(info.seckey,"sdfasdjfhkwejflkaj");
    sqlObj.writeSecKey(&info);

    sqlObj.closeDB();

    return 0;
}