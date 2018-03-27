/**
 * 二进制信号量
 * 在主线程中，我们等到直到有文本的输入，然后调用sem_post增加信号量的值，这将立刻令另一个线程从sem_wait等待中返回并开始执行。
 * 在统计完字符个数之后，再次调用sem_wait并再次被阻塞，直到主线程sem_post增加信号量的值
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
hello world
you input 11 characters
12345
you input 5 characters
end

waiting for thread to finish...
thread joined,it returned
*/