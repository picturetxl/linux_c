
#include <stdlib.h>
#include <stdio.h>

#include "mysql.h"

MYSQL my_connection;
MYSQL_RES *res_ptr;//指向结果集结构的指针
MYSQL_ROW sqlrow;//typedef char **MYSQL_ROW;		/* return data as array of strings */----> 指向一个行结构
void display_row();
int main(int argc, char const *argv[])
{

    int res;
    //如果传递的是已有的结构，则会初始化这个结构
    mysql_init(&my_connection);
   
    //参数：连接句柄 主机名或者ip地址 用户名 密码 数据库名
    if ( mysql_real_connect(&my_connection,"localhost","txl","secret","test",0,NULL,0))
    {
        printf("Connection success\n");
        res=mysql_query(&my_connection,"select childno,fname,age from children where age >5");
        if(res)
        { 
            printf("select error:%s\n",mysql_error(&my_connection));
        }
        else//如果有数据 res不为0
        {
            res_ptr=mysql_use_result(&my_connection);//此函数一次提取一行数据 ，他返回一个指向结果集结构的指针（只是为将提取的数据放到他初始化的结果集中）。如果失败 返回NULL
            if(res_ptr)
            {
                printf("fecth data...\n");
                //mysql_fetch_row()函数将mysql_store_result()得到的结果结构中提取一行，并把他放入一个行结构中 当数据用完或者发生错误时返回NULL
                while( (sqlrow=mysql_fetch_row(res_ptr)) )
                {   
                    display_row();
                }
                if (mysql_errno(&my_connection))
                {
                    fprintf(stderr,"Retrive error:%s\n",mysql_error(&my_connection));
                }
                mysql_free_result(res_ptr);
            }
           
               
        }
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
void display_row()
{
    unsigned int field_count;
    field_count=0;
    while(field_count<mysql_field_count(&my_connection))
    {
        printf("%s ",sqlrow[field_count]);
        field_count++;
    }
    printf("\n");
}
/**OutPut

 */