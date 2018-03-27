#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE*read_fp;
    char buffer[BUFSIZ+1];
    int char_read;
    memset(buffer,'\0',sizeof(buffer));
    read_fp=popen("ps ax","r");
    if(read_fp!=NULL)
    {
        char_read=fread(buffer,sizeof(char),BUFSIZ,read_fp);//read_fp-->buffer
        //连续的从文件流中读取数据，直到没有数据可读
        while(char_read>0)
        {
           buffer[char_read-1]='\0';
           printf("reading %d -\n %s\n",BUFSIZ,buffer);
           char_read=fread(buffer,sizeof(char),BUFSIZ,read_fp);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    //read_fp==NULL
    exit(EXIT_FAILURE);
}
/**Output

*/