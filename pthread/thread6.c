/**
 * 设置线程的优先级
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void*arg);
char message[]="hello world";
int thread_finished=0;
int max_priority;
int min_priority;
struct sched_param scheduling_value;
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    pthread_attr_t thread_attr;
    //创建一个线程属性
    res=pthread_attr_init(&thread_attr);
    if(res!=0)
    {
        perror("pthread_attr_init failed\n");
        exit(EXIT_FAILURE);
    }
    //设置为detach状态 脱离状态：主线程不再等待子线程 
    res=pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);
    if(res!=0)
    {
        perror("pthread_attr_setdetachstate failed\n");
        exit(EXIT_FAILURE);
    }
    //设置调度策略
    res=pthread_attr_setschedpolicy(&thread_attr,SCHED_OTHER);
    if(res!=0)
    {
        perror("pthread_attr_setschedparam failed\n");
        exit(EXIT_FAILURE);
    }
    //查找允许的优先级范围
    max_priority=sched_get_priority_max(SCHED_OTHER);
    min_priority=sched_get_priority_min(SCHED_OTHER);
    //设置优先级
    scheduling_value.__sched_priority=min_priority;
    res=pthread_attr_setschedparam(&thread_attr,&scheduling_value);
    if(res!=0)
    {
        perror("pthread_attr_setschedparam failed\n");
        exit(EXIT_FAILURE);
    }
    //设置属性 &thread_attr
    res=pthread_create(&a_thread,&thread_attr,thread_function,(void *)message);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }
    (void)pthread_attr_destroy(&thread_attr);
    //可以看出仍然共享这变量
    while(!thread_finished)
    {
        printf("waiting for thread to say it's finished....\n");
        sleep(1);
    }
    printf("waiting for thread to finish...\n");
   
    printf("other thread finished,bye!\n");
    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    printf("thread function is running .argument was %s\n",(char *)arg);
    sleep(4);
    printf("Second thread setting finished flag,and exiting now\n");
    thread_finished=1;
    pthread_exit(NULL);
}

/**OuPut
waiting for thread to say it's finished....
thread function is running .argument was hello world
waiting for thread to say it's finished....
waiting for thread to say it's finished....
waiting for thread to say it's finished....
Second thread setting finished flag,and exiting now
waiting for thread to finish...
other thread finished,bye!
*/