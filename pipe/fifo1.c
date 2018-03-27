#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
    int res=mkfifo("/tmp/my_fifo",0777);
    if(res==0)
    {
        printf("fifo create\n");
    }
    exit(EXIT_SUCCESS);
}

 
/*$ ls -lF my_fifo    //p代表是个管道 最后的|也代表是个管道 -F的功劳: append indicator (one of * / = > @|) to entries
prwxrwxr-x 1 tailiang tailiang 0 3月  27 09:33 my_fifo|
-------------------------------------------------------------
Output:
fifo create
*/