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
    //uname -a 输出的是系统信息，包括计算机型号，操作系统名称 版本 发行号 计算机的网络名
    read_fp=popen("uname -a","r");//r表示可以对uname -a的输出进行操作
    if(read_fp!=NULL)
    {
        char_read=fread(buffer,sizeof(char),BUFSIZ,read_fp);//read_fp-->buffer
        if(char_read>0)
        {
            printf("Output was:-\n%s\n",buffer);
        }
        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    //read_fp==NULL
    exit(EXIT_FAILURE);
}
/**Output
Output was:-
Linux tailiang 4.13.0-37-generic #42~16.04.1-Ubuntu SMP Wed Mar 7 16:03:28 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

*/