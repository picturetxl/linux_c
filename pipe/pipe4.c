#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    
    int data_processed;
    char buffer[BUFSIZ+1];
    int file_descriptor;

    memset(buffer,'\0',sizeof(buffer));
    //argv[1] :file_pipes[0]
    sscanf(argv[1],"%d",&file_descriptor);
    data_processed=read(file_descriptor,buffer,BUFSIZ);
    printf("\n%d read %d bytes:%s\n",getpid(),data_processed,buffer);
}
