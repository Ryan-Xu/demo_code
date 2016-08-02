#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define MAXLINE 1024
#define DEF_PAGER "/bin/more" /* default pager program */

int
main(int argc, char *argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char *pager, *argv0;
    char line[MAXLINE];
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage: pipedemo <filename>\n");
        exit(0);
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "cant't open %s\n", argv[1]);
         exit(-1);
    }

    if (pipe(fd) < 0)
        perror("pipe error");

    if ((pid = fork()) < 0) {
         perror("fork error");
         exit(-1);
    } else if (pid > 0) { /* parent */
        close(fd[0]); /* close read end */

        /* parent copies argv[1] to pipe */
        while (fgets(line, MAXLINE, fp) != NULL) {
             n = strlen(line);
             if (write(fd[1], line, n) != n)
                 fprintf(stderr, "write error to pipe\n");
        }
        if (ferror(fp))
            perror("fgets error");

        close(fd[1]); /* close writer end of pipe for reader */

        if (waitpid(pid, NULL, 0) < 0)
            perror("waitpid error");

        exit(0);
    } else { /* child */
        close(fd[1]); /* close write end */
        if (fd[0] != STDIN_FILENO) {
             if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                 fprintf(stderr, "dup2 error to stdin\n");
             close(fd[0]); /* don't need this after dup2 */
        }

        /* get arguments for execl() */
        //if ((pager = getenv("PAGER")) == NULL)
            pager = DEF_PAGER;
        if ((argv0 = strrchr(pager, '/')) != NULL)
            argv0++;
        else
            argv0 = pager;

        if (execl(pager, argv0, (char *)0) < 0) {
             fprintf(stderr, "execl error for %s\n", pager);
        }
    }
    exit(0);
}
