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
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if (argc != 2) {
        fprintf(stderr, "usage: pipedemo <filename>\n");
        exit(0);
    }

    if ((fpin = fopen(argv[1], "r")) == NULL) {
         fprintf(stderr, "cant't open %s\n", argv[1]);
         exit(-1);
    }

    if ((fpout = popen(DEF_PAGER, "w")) == NULL) {
         perror("popen error");
         exit(-1);
    }

    while (fgets(line, MAXLINE, fpin) != NULL) {
         if (fputs(line, fpout) == EOF)
             fprintf(stderr, "fputs error to pipe");
    }
    if (ferror(fpin))
        fprintf(stderr, "fgets error");
    if (pclose(fpout) == -1)
        perror("pclose error");

    exit(0);
}
