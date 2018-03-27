//创建一个文件锁

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>


int main()
{
    int file_desc;
    int save_errno;
    
    file_desc=open("/tmp/LCK.test",O_RDWR|O_CREAT|O_EXCL,0444);//umask
    if(file_desc==-1)
    {
        save_errno=errno;//EEXIST 错误号为17 文件已经存在
        printf("Open failed with error %d\n",save_errno);//第二次想要再次创建文件时发现文件已经存在 就无需再次操作
    }
    else
    {
        printf("Open success\n");//第一次执行创建文件成功 
    }
    getchar();
    exit(EXIT_SUCCESS);
    

}

/**
the first run:
Open success
the second run:
Open failed with error 17

*/