#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <unistd.h>
#include "MyLog.h"
#include <string>
using namespace std;

const string DEBUG_FILE  = "mylog.log";
const int MAX_STRING_LEN =  10240;

//Level的名称
const string ICLevelName[] = { "NOLOG", "DEBUG", "INFO", "WARNING", "ERROR" };

MyLog::MyLog(){}
MyLog::~MyLog(){}

int MyLog::Error_GetCurTime(char *strTime)
{
    struct tm*	tm = NULL;
    size_t		timeLen = 0;
    time_t		tTime = time(NULL);

    tm = localtime(&tTime);
    //timeLen = strftime(strTime, 33, "%Y(Y)%m(M)%d(D)%H(H)%M(M)%S(S)", tmTime);
    timeLen = strftime(strTime, 33, "%Y.%m.%d %H:%M:%S", tm);

    return timeLen;
}

int MyLog::Error_OpenFile(int *pf)
{
    char	fileName[1024];
    memset(fileName, 0, sizeof(fileName));

#ifdef WIN32
    sprintf(fileName, "d:\\%s", DEBUG_FILE.data());
#else
    sprintf(fileName, "./log/%s", DEBUG_FILE.data());
#endif
#if 0
    *pf = open(fileName, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (*pf < 0)
    {
        return -1;
    }
#endif
    return 0;
}

void MyLog::Error_Core(const char *file, int line, int level, int status, const char *fmt, va_list args)
{
    char str[MAX_STRING_LEN];
    int	 strLen = 0;
    char tmpStr[64];
    int	 tmpStrLen = 0;
    int  pf = 0;

    //初始化
    memset(str, 0, MAX_STRING_LEN);
    memset(tmpStr, 0, 64);

    //加入LOG时间
    tmpStrLen = Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen += tmpStrLen;

    //加入LOG等级
    tmpStrLen = sprintf(str + strLen, "[%s] ", ICLevelName[level].data());
    strLen += tmpStrLen;

    //加入LOG状态
    if (status != 0)
    {
        tmpStrLen = sprintf(str + strLen, "[ERRNO is %d] ", status);
    }
    else
    {
        tmpStrLen = sprintf(str + strLen, "[SUCCESS] ");
    }
    strLen += tmpStrLen;

    //加入LOG信息
    tmpStrLen = vsprintf(str + strLen, fmt, args);
    strLen += tmpStrLen;

    //加入LOG发生文件
    tmpStrLen = sprintf(str + strLen, " [%s]", file);
    strLen += tmpStrLen;

    //加入LOG发生行数
    tmpStrLen = sprintf(str + strLen, " [%d]\n", line);
    strLen += tmpStrLen;

    //打开LOG文件
    if (Error_OpenFile(&pf))
    {
        return;
    }
#if 0
    //写入LOG文件
    write(pf, str, strLen);
    //IC_Log_Error_WriteFile(str);

    //关闭文件
    close(pf);
#endif
    return;
}

void MyLog::Log(const char *file, int line, int level, int status, const char *fmt, ...)
{
    va_list args;
    //判断是否需要写LOG
    if (level == NOLOG)
    {
        return;
    }

    //调用核心的写LOG函数
    va_start(args, fmt);
    Error_Core(file, line, level, status, fmt, args);
    va_end(args);

    return;
}
