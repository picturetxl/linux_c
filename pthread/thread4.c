/**
 * 每次去操作临界区 都要加锁
 * 1--写的时候 读的时候
 * 2--查看临界区是否为空
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>

void *thread_function(void*arg);
#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int time_to_exit=0;

pthread_mutex_t work_mutex;//互斥信号量
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void *thread_result;
    res=pthread_mutex_init(&work_mutex,NULL);
    if(res!=0)
    {
        perror("thread_mutex_init failed\n");
        exit(EXIT_FAILURE);
    }   
    res=pthread_create(&a_thread,NULL,thread_function,NULL);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }

    //写数据加锁
    pthread_mutex_lock(&work_mutex);
    printf("Input some text.enter 'end' to finish\n");
    while(!time_to_exit)//不是退出标志
    {
        fgets(work_area,WORK_SIZE,stdin);
        pthread_mutex_unlock(&work_mutex);//写完解锁
        //轮循
        while(1)
        {
            pthread_mutex_lock(&work_mutex);//查看work_area是否为空 如果为空写数据 如果不为空就说明有数据 子线程还没有统计结束 睡一秒
            if(work_area[0]!='\0') 
            {
                pthread_mutex_unlock(&work_mutex);
                sleep(1);//让子线程执行
            }
            else
            {
                break;
            }
        }
    }
    pthread_mutex_unlock(&work_mutex);

    printf("waiting for thread to finish...\n");
    //int pthread_join(pthread_t __th, void **__thread_return)
    res=pthread_join(a_thread,&thread_result);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("thread joined\n");
    pthread_mutex_destroy(&work_mutex);
    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    sleep(1);
    //统计数据 加锁
    pthread_mutex_lock(&work_mutex);
    while(strncmp("end",work_area,3)!=0)//不是end
    {
        printf("you input %zu characters\n",strlen(work_area)-1);
        work_area[0]='\0';
        pthread_mutex_unlock(&work_mutex);//统计完解锁
        sleep(1);//让主线程执行
        pthread_mutex_lock(&work_mutex);//查看work_area是否为空 如果为空说明统计完了 睡一秒 如果没有就统计
        //轮循
        while(work_area[0]=='\0')
        {
            pthread_mutex_unlock(&work_mutex);
            sleep(1);//让主线程执行
            pthread_mutex_lock(&work_mutex);
        }
    }
    time_to_exit=1;
    work_area[0]='\0';
    pthread_mutex_unlock(&work_mutex);
    pthread_exit(0);
}

/**OuPut
Input some text.enter 'end' to finish
hello world
you input 11 characters
yes i am john snow
you input 18 characters
end
waiting for thread to finish...
thread joined
*/