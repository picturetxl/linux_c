#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
          *******************************  
          *             *               *
father--->*file_pipes[1]*  file_pipes[0]*-->execl==>pipe4
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
            //将file_pipes[0]保存到buffer
            sprintf(buffer,"%d",file_pipes[0]);
            //要启动的程序 程序名argv[0] 文件描述符argv[1]   (char*)0：终止被调用程序的参数列表
            (void)execl("pipe4","pipe4",buffer,(char*)0);
            exit(EXIT_SUCCESS);
        }//father
        else
        {
            data_processed=write(file_pipes[1],some_data,strlen(some_data));//buffer--->file_pipes[1]
            printf("\n%d wrote %d bytes\n",getpid(),data_processed);
        }
    }
    exit(EXIT_FAILURE);
}
/*Output

4444 wrote 3 bytes

4445 read 3 bytes:123
*/