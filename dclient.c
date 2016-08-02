#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 1234
#define MAXDATASIZE 100

void process(FILE *fp, int sockfd);
char *getMessage(char *sendline, int len, FILE *fp);

int
main(int argc, char *argv[]) {
    int fd;
    struct hostent *he;
    struct sockaddr_in server;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s ipaddr\n", argv[0]);
        exit(1);
    }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() error");
        exit(1);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    //server.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    const char *ip = argv[1];
    server.sin_addr.s_addr = inet_addr(ip);

    if ((connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1) {
         perror("connect() error");
         exit(1);
    }

    process(stdin, fd);

    close(fd);

}


void process(FILE *fp, int sockfd) {
     char sendline[MAXDATASIZE], recvline[MAXDATASIZE];
     int numbytes;

     printf("connected to server. \n");

     printf("input name: ");
     if (fgets(sendline, MAXDATASIZE, fp) == NULL) {
          printf("\n EXit. \n");
          return;
     }
     send(sockfd, sendline, strlen(sendline),0);

     while(getMessage(sendline, MAXDATASIZE, fp) != NULL) {
         send(sockfd, sendline, strlen(sendline), 0);

         if ((numbytes = recv(sockfd, recvline, MAXDATASIZE, 0)) == 0) {
             perror("server terminated\n");
             return;
         }

        recvline[numbytes] = '\0';
        printf("server message: %s\n", recvline);

     }

     printf("\nExit.\n");
}

char *getMessage(char *sendline, int len, FILE *fp) {
    printf("input string to server: ");
    return (fgets(sendline, MAXDATASIZE, fp));
}
