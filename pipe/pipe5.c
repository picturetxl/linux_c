#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
--------------------------------------------------------
--------------file_pipes[0]-----file_pipes[0]-----------
---father-------0---------------------0----------child--
----------------1---------------------1-----------------
--------------file_pipes[1]-----file_pipe[1]------------

               ||||||
               ||||||
               \\\///
                \\//
                 \/
--------------------------------------------------------
--------------*************-----file_pipes[0]-----------
---father-------*---------------------0----------child--
----------------1---------------------*-----------------
--------------file_pipes[1]-----************------------             
*/
int main(int argc, char const *argv[])
{
    int data_processed;
    int file_pipes[2];//利用两个文件描述符来创建一个管道
    const char some_data[]="123";
    char buffer[BUFSIZ+1];
    pid_t fork_result;
    memset(buffer,'\0',sizeof(buffer));
    //pipe成功调用返回0 错误返回-1
    if(pipe(file_pipes)==0)
    {
        fork_result=fork();
        if(fork_result==(pid_t)-1)
        {
            fprintf(stderr,"fork failed\n");
            exit(EXIT_FAILURE);
        }
        //child 
        if(fork_result==(pid_t)0)
        {
           close(0);//关闭了标准输入
           dup(file_pipes[0]);
           close(file_pipes[0]);
           close(file_pipes[1]);
           execlp("od","od","-c",(char *)0);
            exit(EXIT_FAILURE);
        }//father
        else
        {
            close(file_pipes[0]);
            data_processed=write(file_pipes[1],some_data,strlen(some_data));//buffer--->file_pipes[1]
            close(file_pipes[1]);            
            printf("\n%d wrote %d bytes\n",getpid(),data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}
/*Output

4444 wrote 3 bytes

4445 read 3 bytes:123
*/