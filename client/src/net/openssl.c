/* 
    How to compile: gcc -o client-communicator client-communicator.c -lssl
    Add -lcrypto if using #include<openssl/crypto.h>
    Example code: http://fm4dd.com/openssl/sslconnect.shtm
*/ 

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
// RECOMMENDED INSTALL: Install with vcpkg (find instructions on VSCode website)
// ALTERNATE: On Debian/Ubuntu, install package libssl-dev with apt to access ssl.h.
#include <openssl/ssl.h>

#define MAXBUF 1024
#define MSG "GET /index.html HTTP/1.0\r\n\r\n"

int main(int argc, char *argv[])
{
    /* 
    Create framework to establish TLS connections.
    TLS version used will be highest version mutually supported
    by both client and server.
    https://www.openssl.org/docs/manmaster/man3/SSL_CTX_new.html
    */
    const SSL_METHOD *method;
    /* ---------------------------------------------------------- *
   * These function calls initialize openssl for correct work.  *
   * ---------------------------------------------------------- */
    //OpenSSL_add_all_algorithms();
    //ERR_load_BIO_strings();
    //ERR_load_crypto_strings();
    //SSL_load_error_strings();
    method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    /*
    Creates a new SSL structure which is needed to hold the data for a TLS/SSL connection.
    https://www.openssl.org/docs/manmaster/man3/SSL_new.html
    */
    SSL *ssl = SSL_new(ctx);


    int sockfd, bytes_read, bytes_written;
    struct addrinfo hints;
    struct addrinfo *ai_list = NULL;
    struct addrinfo *ai = NULL;
    char buffer[MAXBUF + 1];
    const char *host = "www.google.com";
    const char *port = "443";

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    int ret = getaddrinfo(host, port, &hints, &ai_list);
    if (ret != 0) {
        fprintf(stderr, "getaddrinfo: error\n");
        exit(-1);   
    }

    for (ai = ai_list; ai != NULL; ai = ai->ai_next) {
        sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (sockfd == -1) {
            perror("socket");
            continue;
        }
        
        /* Connect the SSL object with a file descriptor
        https://www.openssl.org/docs/manmaster/man3/SSL_set_fd.html
        */
        ret = SSL_set_fd(ssl, sockfd);
        if (ret != 1) {
            fprintf(stderr, "SSL_set_fd error\n");
            exit(-1);   
        }
        SSL_set_connect_state(ssl);
        if ((ret = SSL_do_handshake(ssl)) != 1) {
            fprintf(stderr, "Handshake failed: %d\n", SSL_get_error(ssl, ret));
        }
        printf("Here\n");
        ret = SSL_connect(ssl);
        if (ret != 1) {
            fprintf(stderr, "SSL_connect error: %d\n", SSL_get_error(ssl, ret));
            exit(-1);   
        }
        /*
        if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) == -1) {
            perror("connect");
            close(sockfd);
            continue;
        }
        */

        break;
    }

    if (ai == NULL) {
        fprintf(stderr, "Failed to find a suitable address for connection\n");
        exit(-1);
    }

    sprintf(buffer, MSG);
    //ret = send(sockfd, buffer, strlen(buffer), 0);
    bytes_written = SSL_write(ssl, buffer, MAXBUF);
    if (bytes_written > 0) {
        printf("Bytes sent %d\n", ret);
    }
    else {
        fprintf(stderr, "SSL_write error: %d\n", SSL_get_error(ssl, bytes_written));
    }
    do
    {
        memset(buffer, 0, sizeof(buffer));
        //bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        bytes_read = SSL_read(ssl, buffer, sizeof(buffer));
        if ( bytes_read > 0 ) {
            printf("%s", buffer);
        }
        else {
            fprintf(stderr, "SSL_read error: %d\n", SSL_get_error(ssl, bytes_read));
        }
    } while ( bytes_read > 0 );

    if (SSL_get_error(ssl, ret) == SSL_ERROR_SYSCALL ||
        SSL_get_error(ssl, ret) == SSL_ERROR_SSL) {
        fprintf(stderr, "Previous SSL error %d preventing call to SSL_shutdown.\n", ret);    
    }
    else {
        if ((ret = SSL_shutdown(ssl)) != 1) {
            fprintf(stderr, "SSL_shutdown error: %d\n", SSL_get_error(ssl, ret));
        }
    }
    close(sockfd);
    if (SSL_get_shutdown(ssl) == SSL_RECEIVED_SHUTDOWN) {
        printf("SSL received shutdown. Exiting...\n");
    }
    return 0;
}
