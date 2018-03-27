#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
          *******************************  
          *             *               *
buffer--->*file_pipes[1]*  file_pipes[0]*-->buffer
          *             *               *
          *******************************
*/
int main(int argc, char const *argv[])
{
    int data_processed;
    int file_pipes[2];//利用两个文件描述符来创建一个管道
    const char some_data[]="123";
    char buffer[BUFSIZ+1];
    memset(buffer,'\0',sizeof(buffer));
    //pipe成功调用返回0 错误返回-1
    if(pipe(file_pipes)==0)
    {
        data_processed=write(file_pipes[1],some_data,strlen(some_data));//buffer--->file_pipes[1]
        printf("wrote %d bytes\n",data_processed);
        data_processed=read(file_pipes[0],buffer,BUFSIZ);//file_pipes[0]-->buffer
        printf("Read %d bytes:%s\n",data_processed,buffer);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
/*Output
wrote 3 bytes
Read 3 bytes:123
*/