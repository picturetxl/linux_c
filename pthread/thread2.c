/**
 * 让主线程去打印1
 * 让子线程去打印2
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
/*为了使用这些函数库，必须定义宏_REENTRANT 包含pthread.h 用-lpthread 去链接线程库*/
#include <pthread.h>

void *thread_function(void*arg);
// char message[]="hello world";

int run_now=1;//两个线程的共享变量
int main(int argc, char const *argv[])
{
    int res;
    int print_count=0;
    pthread_t a_thread;
    void *thread_result;

    res=pthread_create(&a_thread,NULL,thread_function,(void *)NULL);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }
    while(print_count++<20)
    {
        if(run_now==1)
        {
            printf("1");
            run_now=2;
        }
        else
        {
            sleep(1);
        }
    }
    printf("\n");
    printf("waiting for thread to finish...\n");
    //int pthread_join(pthread_t __th, void **__thread_return)
    res=pthread_join(a_thread,&thread_result);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("thread joined,it returned %s\n",(char *)thread_result);
    // printf("Message is now %s\n",message);
    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    int print_count2=0;
    // printf("thread function is running .argument was %s\n",(char *)arg);
    sleep(3);
    // strcpy(message,"Bye!");
    //void pthread_exit(void *__retval)
    while(print_count2++<20)
    {
        if(run_now==2)
        {
            printf("2");
            run_now=1;
        }
        else
        {
            sleep(1);
        }
    }
    pthread_exit("\nthank you for the cpu time");
}

/**OuPut
121212121212121212
waiting for thread to finish...
thread joined,it returned
thank you for the cpu time
*/