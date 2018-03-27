/**
 * 取消线程
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void*arg);
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void *thread_result;

    res=pthread_create(&a_thread,NULL,thread_function,(void *)NULL);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }
    sleep(3);
    printf("canceling thread...\n");
    res=pthread_cancel(a_thread);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("waiting for for thread to finish..\n");
    res=pthread_join(a_thread,&thread_result);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    int res;
    //设置线程可以被取消
    res=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    if(res!=0)
    {
        perror("pthread_setcancelstate failed\n");
        exit(EXIT_FAILURE);
    }
    //设置线程取消类型 type有两种：PTHREAD_CANCEL_DEFERRED 接受到取消请求后一直等待到线程执行了下述的函数之一后才采取行动：pthread_join pthread_cond_wait pthread_cond_timewait pthread_testcancel sem_wait sigwait
    //                         PTHREAD_CANCEL_ASYNCHRONOUS(异步的) 接受到取消请求立即采取行动
    res=pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    if(res!=0)
    {
        perror("pthread_setcanceltype failed\n");
        exit(EXIT_FAILURE);
    }   
    printf("thread function is running .\n");
    for(int i = 0; i < 10  ; i++)
    {
        printf("thread is still running(%d)..\n",i);
        sleep(1);
    }
   
    pthread_exit(0);
}

/**OuPut
thread function is running .
thread is still running(0)..
thread is still running(1)..
thread is still running(2)..
canceling thread...
thread is still running(3)..
waiting for for thread to finish..
*/