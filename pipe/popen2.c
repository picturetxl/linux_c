#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE*write_fp;
    char buffer[BUFSIZ+1];
    sprintf(buffer,"Once upon a time .there was...\n");
    //
    write_fp=popen("od -c","w");//w表示的是将write_fp的内容作为od -c的输入
    if(write_fp!=NULL)
    {
        fwrite(buffer,sizeof(char),strlen(buffer),write_fp);//read_fp-->buffer
        pclose(write_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
/**Output
0000000   O   n   c   e       u   p   o   n       a       t   i   m   e
0000020       .   t   h   e   r   e       w   a   s   .   .   .  \n
0000037
*/