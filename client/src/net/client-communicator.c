#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAXBUF  1024
#define MSG "GET /index.html HTTP/1.0\r\n\r\n"

int main(int argc, char *argv[])
{   
    int sockfd, bytes_read;
    struct addrinfo hints;
    struct addrinfo *ai_list = NULL;
    struct addrinfo *ai = NULL;
    int ret = 0;
    char buffer[MAXBUF + 1];
    const char *host = "www.google.com";
    const char *port = "80";

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    ret = getaddrinfo(host, port, &hints, &ai_list);
    if (ret != 0) {
        printf("getaddrinfo: error\n");
        exit(-1);   
    }

    for (ai = ai_list; ai != NULL; ai = ai->ai_next) {
        sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }
        if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) == -1) {
            perror("connect");
            close(sockfd);
            continue;
        }
        break;
    }
    
    if (ai == NULL) {
        fprintf(stderr, "Failed to find a suitable address for connection\n");
        exit(-1);
    }

    sprintf(buffer, MSG);
    ret = send(sockfd, buffer, strlen(buffer), 0);
    printf("Bytes sent %d\n", ret);
    do
    {
        memset(buffer, 0, sizeof(buffer));
        bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if ( bytes_read > 0 )
            printf("%s", buffer);
    }
    while ( bytes_read > 0 );

    close(sockfd);
    return 0;
}