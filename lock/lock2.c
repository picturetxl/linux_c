//此程序有问题
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

const char *lock_file="/tmp/LCK.test2";
int main()
{
    int file_desc;
    int tries=10;

    while(tries--)
    {
        file_desc=open(lock_file,O_RDWR|O_CREAT|O_EXCL,0444);
        if(file_desc==-1)
        {
            printf("%d - lock already present \n",getpid());
            sleep(3);
        }
        else//临界区
        {
            printf("%d - i have exclisive access\n",getpid());
            sleep(1);
            (void)close(file_desc);//终止文件描述符file_desc与其对应文件之间的关联 文件描述符被释放并可以重新使用
            (void)unlink(lock_file);//使文件引用数减一
            sleep(2);
        }
    }
    exit(EXIT_SUCCESS);
}