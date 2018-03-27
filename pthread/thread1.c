/**
 * <<linux程序设计>>
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/*为了使用这些函数库，必须定义宏_REENTRANT 包含pthread.h 用-lpthread 去链接线程库*/
#include <pthread.h>

void *thread_function(void*arg);
char message[]="hello world";
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void *thread_result;

    res=pthread_create(&a_thread,NULL,thread_function,(void *)message);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("waiting for thread to finish...\n");
    //int pthread_join(pthread_t __th, void **__thread_return)
    res=pthread_join(a_thread,&thread_result);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("thread joined,it returned %s\n",(char *)thread_result);
    printf("Message is now %s\n",message);
    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    printf("thread function is running .argument was %s\n",(char *)arg);
    sleep(3);
    /*code*/
    strcpy(message,"Bye!");//hello world-->Bye
    //void pthread_exit(void *__retval)
    pthread_exit("thank you for the cpu time");
}

/**OuPut
waiting for thread to finish...
thread function is running .argument was hello world
thread joined,it returned thank you for the cpu time
Message is now Bye!
*/