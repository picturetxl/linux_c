#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char const *argv[])
{
    
    int res;
    int open_mode=0;
    int i;

    if(argc<2)
    {
        fprintf(stderr,"usage: %s <some combination of O_RDONLY O_WRONLY O_NONBLOCK>\n",*argv);
        exit(EXIT_FAILURE);
    }    
    //比较命令行参数
    for(i=1;i<argc;i++)
    {
        if(strncmp(*++argv,"O_RDONLY",8)==0) //*(++argv)
        {
            open_mode|= O_RDONLY;
        }
        if(strncmp(*argv,"O_WRONLY",8)==0)
        {
            open_mode|= O_WRONLY;
        }
        if(strncmp(*argv,"O_NONBLOCK",10)==0)
        {
            open_mode|= O_NONBLOCK;
        }
    }
    //检查fifo文件是否存在 如果不存在就创建
    if(access(FIFO_NAME,F_OK)==-1)
    {
        res=mkfifo(FIFO_NAME,0777);
        if(res!=0)
        {
            fprintf(stderr,"could not create fifo %s\n",FIFO_NAME);
            exit(EXIT_FAILURE);
        }
    }
    //打开fifo文件 但是到最后也没有删除他 因为无法确定别的程序是否在使用他
    printf("Process %d opening fifo\n",getpid());
    /*    --open_flag
        O_RDONLY :open调用将阻塞 除非有一个进程以写的方式打开同一个fifo，否则ta不会返回
        O_WRONLY :open调用将阻塞 除非有一个进程以读的方式打开同一个fifo，否则ta不会返回
        O_NONBLOCK : open调用将立即返回 不阻塞
    */
    res=open(FIFO_NAME,open_mode);
    printf("process %d result %d\n",getpid(),res);
    sleep(5);
    if(res!=0)
    {
        (void)close(res);
    }
    printf("process %d finished\n",getpid());
    exit(EXIT_SUCCESS);
}
/**run:
 * ./fifo2 O_RDONLY &  先启动读进程  并且在open调用中等待
 * ./fifo2 O_WRONLY    
 */
/**Output
Process 5527 opening fifo
process 5527 result 3
process 5526 result 3
process 5526 finished
process 5527 finished
[1]+  Done    
*/