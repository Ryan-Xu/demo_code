#include <unistd.h>
#include <stdio.h>

int glob = 6;
char buf[] = "a write to stdou\n";

int
main()
{
    int var;
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO,buf, sizeof(buf)-1) != sizeof(buf)-1)
        perror("write error");

    printf("before fork\n");

    if ((pid = fork()) < 0) {
        perror("fork error");
    }
    else if (pid == 0) { /* child */
        glob++;
        var++;
    }
    else {
         sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    exit(0);
}
