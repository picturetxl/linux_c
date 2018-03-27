/**
 * 跨越fork进程的管道操作
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
          *******************************  
          *             *               *
father--->*file_pipes[1]*  file_pipes[0]*-->child
          *             *               *
          *******************************
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
        if(fork_result==-1)
        {
            fprintf(stderr,"fork failed\n");
            exit(EXIT_FAILURE);
        }
        //child 
        if(fork_result==0)
        {
            data_processed=read(file_pipes[0],buffer,BUFSIZ);//file_pipes[0]-->buffer
            printf("Read %d bytes:%s\n",data_processed,buffer);
            exit(EXIT_SUCCESS);
        }//father
        else
        {
            data_processed=write(file_pipes[1],some_data,strlen(some_data));//buffer--->file_pipes[1]
            printf("wrote %d bytes\n",data_processed);
        }
    }
    exit(EXIT_FAILURE);
}
/*Output
wrote 3 bytes
Read 3 bytes:123
*/