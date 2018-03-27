/**
 * 需要lock4
 * 在一个文件当中创建字节区域锁
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
    int byte_count;
    char *byte_to_write="A";
    struct flock region_1;
    struct flock region_2;
    int res;

    //将文件设置为读写的方式 这样既可以共享锁 有可以独占锁
    //读写方式 两种锁都支持
    //如果是读锁 则将设置为读的方式 
    //如果是写锁 则将设置为写的方式
    file_desc=open(test_file,O_RDWR|O_CREAT,0666);
    if(!file_desc)
    {
        fprintf(stderr,"Unable to open %s for read/write",test_file);
        exit(EXIT_FAILURE);
    }
    //写数据
    for(byte_count=0;byte_count<100;byte_count++)
    {
        (void)write(file_desc,byte_to_write,1);
    }
    //共享锁（读）
    region_1.l_type=F_RDLCK;
    region_1.l_whence=SEEK_SET;//文件开始
    region_1.l_start=10;
    region_1.l_len=20;
    //独占锁（写）
    region_2.l_type=F_WRLCK;
    region_2.l_whence=SEEK_SET;
    region_2.l_start=40;
    region_2.l_len=10;

    printf("Process %d locking file\n",getpid());

    //-----------设置锁文件-----------------
    //设置region_1
    res=fcntl(file_desc,F_SETLK,&region_1);
    if(res==-1)
    {
        fprintf(stderr,"Failed to lock region 1\n");
    }
    //设置region_2
    res=fcntl(file_desc,F_SETLK,&region_2);
    if(res==-1)
    {
        fprintf(stderr,"Failed to lock region 2\n");
    }
    sleep(60);
    printf("Process %d closing file\n",getpid());
    close(file_desc);
    exit(EXIT_SUCCESS);
}