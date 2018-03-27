#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"
int main(int argc, char const *argv[])
{
    MYSQL* conn_ptr;
    //传递NULL给init函数，他会返回一个指向新分配的连接句柄的指针 出错会返回NULL
    conn_ptr=mysql_init(NULL);
    if(!conn_ptr)
    {
        fprintf(stderr,"mysql_init failed\n");
        return EXIT_FAILURE;
    }
    //参数：连接句柄 主机名或者ip地址 用户名 密码 数据库名
    conn_ptr=mysql_real_connect(conn_ptr,"localhost","txl","iii","test",0,NULL,0);
    if (conn_ptr)
    {
        printf("Connection success\n");
        /*coding or operation*/
    }
    else
    {
        printf("Connection dailed\n");
    }
    //关闭连接
    mysql_close(conn_ptr);

    return EXIT_SUCCESS;
}
