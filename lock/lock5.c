/**
 * 链接lock3
 * 锁的竞争
 * 共享锁可以多把
 * 有共享锁不可以加独占锁
 * 有独占锁也不可以加共享锁
 */
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

const char *test_file="/tmp/test_lock";
int main()
{
    int file_desc;
    struct flock region_to_lock;
    int res;

    file_desc=open(test_file,O_RDWR|O_CREAT,0666);
    if(!file_desc)
    {
        fprintf(stderr,"Unable to open %s for read/write",test_file);
        exit(EXIT_FAILURE);
    }

    //将10-15字节上加共享锁
    region_to_lock.l_type=F_RDLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=10;
    region_to_lock.l_len=5;
    printf("Process %d trying F_RDLCk,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));
    res=fcntl(file_desc,F_SETLK,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to lock region\n",getpid());
    }
    else{
         printf("Process %d -obtained  lock region\n",getpid());
    }

    //将10-15字节上解除之前加的共享锁 只是解除本程序对10-15字节上的共享锁 实际上测试文件本身共享锁依然存在
    region_to_lock.l_type=F_UNLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=10;
    region_to_lock.l_len=5;
    printf("Process %d trying F_UNLCK,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));
    res=fcntl(file_desc,F_SETLK,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to unlock region\n",getpid());
    }
    else{
         printf("Process %d -unlock region\n",getpid());
    }

    //解锁0-50字节上的锁 但是测试文件本身的锁依然存在
    region_to_lock.l_type=F_UNLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=0;
    region_to_lock.l_len=50;
    printf("Process %d trying F_UNLCK,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));
    res=fcntl(file_desc,F_SETLK,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to unlock region\n",getpid());
    }
    else{
         printf("Process %d -unlock region\n",getpid());
    }

    //将16-21字节上加独占锁 但是测试文件本身已经有了共享锁 所以失败
    region_to_lock.l_type=F_WRLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=16;
    region_to_lock.l_len=5;
    printf("Process %d trying F_WRLCK,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));
    res=fcntl(file_desc,F_SETLK,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to lock region\n",getpid());
    }
    else{
         printf("Process %d -obtained lock region\n",getpid());
    }

    //将40-50字节上添加共享锁 但是测试文件本身已经有了独占锁的 所以也失败了
    region_to_lock.l_type=F_RDLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=40;
    region_to_lock.l_len=10;
    printf("Process %d trying F_RDLCk,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));

    res=fcntl(file_desc,F_SETLK,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to lock region\n",getpid());
    }
    else{
         printf("Process %d -obtained  lock region\n",getpid());
    }


    //
    region_to_lock.l_type=F_WRLCK;
    region_to_lock.l_whence=SEEK_SET;//文件开始
    region_to_lock.l_start=16;
    region_to_lock.l_len=5;

    printf("Process %d trying F_WRLCK,region %d to %d\n",getpid(),(int)region_to_lock.l_start,(int)(region_to_lock.l_len+region_to_lock.l_start));
    //F_SETLKW 命令 等待直到它可以获得一把锁为止 
    //于是程序的输出会遇到一个比较长的停顿，直到已经锁住这块区域的lock3完成sleep调用，关闭文件而释放了他之前获得的所有锁为止
    //lock5继续执行成功锁定这块区域，最后也退出运行
    res=fcntl(file_desc,F_SETLKW,&region_to_lock);
    if(res==-1)
    {
        printf("Process %d -failed to lock region\n",getpid());
    }
    else{
         printf("Process %d -obtained lock region\n",getpid());
    }


    printf("Process %d ending\n",getpid());
    close(file_desc);
    exit(EXIT_SUCCESS);
}

/**
 * run:
 * ./lock3 &
 * 
 * ./lock5
 */
/**
Process 6280 trying F_RDLCk,region 10 to 15
Process 6280 -obtained  lock region
Process 6280 trying F_UNLCK,region 10 to 15
Process 6280 -unlock region
Process 6280 trying F_UNLCK,region 0 to 50
Process 6280 -unlock region
Process 6280 trying F_WRLCK,region 16 to 21
Process 6280 -failed to lock region
Process 6280 trying F_RDLCk,region 40 to 50
Process 6280 -failed to lock region
Process 6280 trying F_WRLCK,region 16 to 21
Process 6279 closing file
Process 6280 -obtained lock region
Process 6280 ending
[1]+  Done                    ./lock3

*/