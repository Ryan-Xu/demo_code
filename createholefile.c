#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main()
{
    int fd;
    if ((fd = creat("file.hole", 0755)) < 0)
        perror("create error");
    
    if (write(fd, buf1, 10) != 10)
        perror("buf1 write error");
        
    if (lseek(fd, 16384, SEEK_SET) == -1)
        perror("lseek error");
        
    if (write(fd, buf2, 10) != 10)
        perror("buf2 write error");
        
    exit(0);

}
