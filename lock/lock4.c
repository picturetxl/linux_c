/**
 * 链接lock3
 * 测试一个文件上的已有锁
 */
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

const char *test_file="/tmp/test_lock";
#define SIZE_TO_TRY 5

void show_lock_info(struct flock *to_show);

int main()
{
    int file_desc;
    int res;
    struct flock region_to_test;
    int start_byte;
    file_desc=open(test_file,O_RDWR | O_CREAT,0666);
    if(!file_desc)
    {
        fprintf(stderr,"Unable to open %s for read/write",test_file);
        exit(EXIT_FAILURE);
    }
    //------------------每五个字节一测 看锁类型---------------------
    for(start_byte=0;start_byte<99;start_byte+=SIZE_TO_TRY)
    {
        //××××××××××××测试写锁××××××××××××××××××××××
        region_to_test.l_type=F_WRLCK;
        region_to_test.l_whence=SEEK_SET;
        region_to_test.l_start=start_byte;
        region_to_test.l_len=SIZE_TO_TRY;
        region_to_test.l_pid=-1;//设置持有锁的进程的默认值为-1 一旦这个值被改变 则证明flock结构被修改过

        printf("Testing F_WRLCK on region from %d to %d\n",start_byte,start_byte+SIZE_TO_TRY);

        res=fcntl(file_desc,F_GETLK,&region_to_test);
        if(res==-1)
        {
            fprintf(stderr,"F_GETLCK failed\n");
            exit(EXIT_FAILURE);
        }
        //是否修改过
        if(region_to_test.l_pid!=-1)
        {
            printf("lcok would faile.F_GETLK returned:\n");
            show_lock_info(&region_to_test);
        }
        else
        {
            printf("F_WRLCK -Lock would successed\n");

        }
         //××××××××××××测试读锁××××××××××××××××××××××
        region_to_test.l_type=F_RDLCK;
        region_to_test.l_whence=SEEK_SET;
        region_to_test.l_start=start_byte;
        region_to_test.l_len=SIZE_TO_TRY;
        region_to_test.l_pid=-1;
        printf("Testing F_RDLCK on region from %d to %d\n",start_byte,start_byte+SIZE_TO_TRY);

        res=fcntl(file_desc,F_GETLK,&region_to_test);
        if(res==-1)
        {
            fprintf(stderr,"F_GETLCK failed\n");
            exit(EXIT_FAILURE);
        }

        if(region_to_test.l_pid!=-1)
        {
            printf("lcok would faile.F_GETLK returned:\n");
            show_lock_info(&region_to_test);
        }
        else
        {
            printf("F_RDLCK -Lock would successed\n");
        }
    }
    close(file_desc);
    exit(EXIT_SUCCESS);
}
void show_lock_info(struct flock *to_show)
{
    printf("\t l_type %d, ",to_show->l_type);
    printf("\t l_whence %d, ",to_show->l_whence);
    printf("\t l_start %d, ",(int)to_show->l_start);//long
    printf("\t l_len %d, ",(int)to_show->l_len);//long
    printf("\t l_pid %d\n ",to_show->l_pid);
}
/**run :
    ./lock3 &
    ./lock4
    */
/**
 * Testing F_WRLCK on region from 0 to 5
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 0 to 5
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 5 to 10
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 5 to 10
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 10 to 15
lcok would faile.F_GETLK returned:
         l_type 0,       l_whence 0,     l_start 10,     l_len 20,       l_pid 5592
 Testing F_RDLCK on region from 10 to 15
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 15 to 20
lcok would faile.F_GETLK returned:
         l_type 0,       l_whence 0,     l_start 10,     l_len 20,       l_pid 5592
 Testing F_RDLCK on region from 15 to 20
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 20 to 25
lcok would faile.F_GETLK returned:
         l_type 0,       l_whence 0,     l_start 10,     l_len 20,       l_pid 5592
 Testing F_RDLCK on region from 20 to 25
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 25 to 30
lcok would faile.F_GETLK returned:
         l_type 0,       l_whence 0,     l_start 10,     l_len 20,       l_pid 5592
 Testing F_RDLCK on region from 25 to 30
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 30 to 35
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 30 to 35
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 35 to 40
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 35 to 40
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 40 to 45
lcok would faile.F_GETLK returned:
         l_type 1,       l_whence 0,     l_start 40,     l_len 10,       l_pid 5592
 Testing F_RDLCK on region from 40 to 45
lcok would faile.F_GETLK returned:
         l_type 1,       l_whence 0,     l_start 40,     l_len 10,       l_pid 5592
 Testing F_WRLCK on region from 45 to 50
lcok would faile.F_GETLK returned:
         l_type 1,       l_whence 0,     l_start 40,     l_len 10,       l_pid 5592
 Testing F_RDLCK on region from 45 to 50
lcok would faile.F_GETLK returned:
         l_type 1,       l_whence 0,     l_start 40,     l_len 10,       l_pid 5592
 Testing F_WRLCK on region from 50 to 55
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 50 to 55
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 55 to 60
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 55 to 60
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 60 to 65
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 60 to 65
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 65 to 70
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 65 to 70
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 70 to 75
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 70 to 75
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 75 to 80
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 75 to 80
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 80 to 85
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 80 to 85
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 85 to 90
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 85 to 90
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 90 to 95
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 90 to 95
F_RDLCK -Lock would successed
Testing F_WRLCK on region from 95 to 100
F_WRLCK -Lock would successed
Testing F_RDLCK on region from 95 to 100
F_RDLCK -Lock would successed
*/