/*
 * @version: 1.0
 * @Author: binlongzhang binlong_zhang@163.com
 * @Date: 2023-04-22 10:41:39
 * @LastEditors: binlongzhang binlong_zhang@163.com
 * @LastEditTime: 2023-04-25 03:08:29
 */

#include <mysql/mysql.h>
#include <stdio.h>
#include <string>

const std::string IP = "localhost";
const std::string USER = "zhangbinglong";
const std::string PWD = "1094859023";

int main(void)
{

    // 初始化客户端库
    if (mysql_library_init(0, NULL, NULL))
    {
        fprintf(stderr, "could not initialize MySQL client library\n");
        exit(1);
    }

    /* 连接 */
    MYSQL mysql;
    mysql_init(&mysql);
    // mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"your_prog_name");
    if (!mysql_real_connect(&mysql,IP.data(), USER.data(), PWD.data(), "ssl_proj", 0, NULL, 0))
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(&mysql));
        return mysql_errno(&mysql);
    }

    /*查询*/
    char *psql = "select * from secnode";
    if (mysql_query(&mysql, psql))
    {
        fprintf(stderr, "Failed to '%s': Error: %s\n", psql, mysql_error(&mysql));
        return mysql_errno(&mysql);
    }

    /*获取结果*/
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
    {
        fprintf(stderr, "Failed to get result: Error: %s\n", mysql_error(&mysql));
        return mysql_errno(&mysql);
    }
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    unsigned int i;

    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    // 打印表头
    for (i = 0; i < num_fields; i++)
    {
        printf("%s\t",fields[i].name);
    }
    printf("\n");

    // 打印结果
    while ((row = mysql_fetch_row(result)))
    {
        unsigned long *lengths = mysql_fetch_lengths(result);
        for (i = 0; i < num_fields; i++)
        {
            printf("[%.*s]\t", (int)lengths[i], row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
    mysql_free_result(result);

    mysql_close(&mysql);
    mysql_library_end();

    return 0;
}