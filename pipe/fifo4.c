#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF

int main(int argc, char const *argv[])
{
    int pipe_fd;
    int res;
    int open_mode=O_RDONLY;
    int bytes_read=0;
    char buffer[BUFFER_SIZE+1];
    memset(buffer,'\0',sizeof(buffer));

    printf("process %d opening fifo O_RDONLY\n",getpid());
    pipe_fd=open(FIFO_NAME,open_mode);
    printf("process %d result %d\n",getpid(),pipe_fd);

    if(pipe_fd!=-1)
    {
        do{
            res=read(pipe_fd,buffer,BUFFER_SIZE);
            bytes_read+=res;
        }while(res>0);
    
        (void)close(pipe_fd);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished,%d bytes read\n \n",getpid(),bytes_read);
    exit(EXIT_SUCCESS);
}
/**Running:
 * ./fifo3 &  //生产者
 * time ./fifo4 //消费者
 */
//--------读取10MB的数据 不到0.003s 效率很高
/**Output
process 6007 opening fifo O_RDONLY
process 6007 result 3
process 6006 result 3
write error on pipe
Process 6007 finished,4096 bytes read

[1]+  Exit 1                  ./fifo3

real    0m0.003s
user    0m0.003s
sys     0m0.002s
*/