/**
 * 二进制信号量
 * 必须得考虑时序
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
void *thread_function(void*arg);
sem_t bin_sem;
#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread;
    void *thread_result;
    //int sem_init(sem_t *__sem, int __pshared, unsigned int __value) value设置为0 表示信号量初始值为0
    res=sem_init(&bin_sem,0,0);
    if(res!=0)
    {
        perror("semphore initialization failed\n");
        exit(EXIT_FAILURE);
    }
    res=pthread_create(&a_thread,NULL,thread_function,(void *)NULL);
    if(res!=0)
    {
        perror("thread creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Input some text.enter 'end' to finish\n");
    while(strncmp("end",work_area,3)!=0)
    {
        //减少输入时间
        if(strncmp(work_area,"FAST",4)==0)
        {
            sem_post(&bin_sem);//+1
            strcpy(work_area,"Wheeee...");
        }
        fgets(work_area,WORK_SIZE,stdin);
        sem_post(&bin_sem);//+1
    }
    printf("\nwaiting for thread to finish...\n");
    //int pthread_join(pthread_t __th, void **__thread_return)
    res=pthread_join(a_thread,&thread_result);
    if(res!=0)
    {
        perror("thread join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("thread joined,it returned\n");
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);
}
//固定格式的函数：参数 返回值都是void*
void *thread_function(void*arg)
{
    sem_wait(&bin_sem);// 阻塞并等待信号量变成非0值 -1
    while(strncmp("end",work_area,sizeof("end")!=3))
    {
        //size_t 使用%zu
        printf("you input %zu characters\n",strlen(work_area)-1);
        sem_wait(&bin_sem);
    }
    pthread_exit(NULL);
}


/**OuPut
Input some text.enter 'end' to finish
hello
you input 5 characters
FAST
you input 8 characters---------------------------------------error
you input 8 characters
end

waiting for thread to finish...
thread joined,it returned
*/