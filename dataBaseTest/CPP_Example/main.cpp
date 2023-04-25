/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-24 11:57:44
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 02:37:51
 */
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <iostream>
// #define DBHOST "tcp://127.0.0.1:3306"
// #define USER "zhangbinglong"
// #define PASSWORD "1094859023"

int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;

    driver = sql::mysql::get_mysql_driver_instance();
    conn = driver->connect("tcp://localhost:3306", "zhangbinglong", "1094859023");
    if (conn->isValid())
    {
        std::cout << "连接成功！" << std::endl;
    }
    else
    {
        std::cout << "连接失败！" << std::endl;
    }

    delete conn;
    driver = NULL;
    conn = NULL;
    return 0;
}
