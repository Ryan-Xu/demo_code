#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    FILE *fp;
    char str[8] = "abcdefg";
    fp = fopen("testio", "a+b");
    if (fputs(str, fp) == EOF)
        perror("fputs error");
    
    fclose(fp);
    exit(0);
}