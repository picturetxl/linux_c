
/**
 * mysql_real_connect()函数一旦失败是直接返回NULL 没有错误代码
 * 所以为了解决这个问题
 * 将MYSQL这个结构的指针传递给他
 * 即使失败了 也能得到相应的错误码，就是可以处理这个错误
 */
#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"
int main(int argc, char const *argv[])
{
    MYSQL my_connection;
    //如果传递的是已有的结构，则会初始化这个结构
    mysql_init(&my_connection);
   
    //参数：连接句柄 主机名或者ip地址 用户名 密码 数据库名
    if ( mysql_real_connect(&my_connection,"localhost","txl","99","test",0,NULL,0))
    {
        printf("Connection success\n");
        mysql_close(&my_connection);
    }
    else
    {
         fprintf(stderr,"Connection failed\n");
        //unsigned int mysql_errno(MYSQL *connection);-->%d
        //char *mysql_error(MYSQL *connection);--------->%s
        if(mysql_errno(&my_connection))
        {
            fprintf(stderr,"Connection error %d: %s\n",mysql_errno(&my_connection),mysql_error(&my_connection));
        }
    }

    return EXIT_SUCCESS;
}

/**OutPut
 * 故意将用户txl的密码设置错误
Connection failed
Connection error 1045: Access denied for user 'txl'@'localhost' (using password: YES)
 */


