#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_THREADS 6
void pthread_function(void*arg);
int main(int argc, char const *argv[])
{
    int res;
    pthread_t a_thread[NUM_THREADS];
    void *thread_result;
    int lots_of_threads;

    for(lots_of_threads=0;lots_of_threads<NUM_THREADS;lots_of_threads++)
    {   //传入的参数是(void*)lots_of_threads
        res=pthread_create( &(a_thread[lots_of_threads]) ,NULL,pthread_function, (void*)lots_of_threads );
        if(res!=0)
        {
            perror("pthread_create failed\n");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    
    printf("waiting for threads to finished...\n");
    for(lots_of_threads=NUM_THREADS-1;lots_of_threads>=0;lots_of_threads--)
    {
        res=pthread_join(a_thread[lots_of_threads],&thread_result);
        if(res==0)
        {
            printf("Picked up a thread\n");
        }
        else
        {
            perror("pthread_join failed\n");
        }
         
    }
    printf("all done\n");
    exit(EXIT_SUCCESS);
}
void pthread_function(void*arg)
{
    int my_number=(int)arg;
    int rand_num;

    printf("thread_function is running .argument was %d\n",my_number);
    rand_num=1+(int)(9.0*rand()/(RAND_MAX+1.0));
    sleep(rand_num);
    printf("bye from %d\n",my_number);
    pthread_exit(NULL);
}
/**OutPut
thread_function is running .argument was 0
thread_function is running .argument was 1
thread_function is running .argument was 2
thread_function is running .argument was 3
thread_function is running .argument was 4
bye from 1
thread_function is running .argument was 5
waiting for threads to finished...
bye from 5
Picked up a thread
bye from 0
bye from 2
bye from 3
bye from 4
Picked up a thread
Picked up a thread
Picked up a thread
Picked up a thread
Picked up a thread
all done
*/