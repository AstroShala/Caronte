//
// Created by simone_mancini on 07/10/16.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>
#include "error_management.h"
#include "send_recv.h"
#include "server_http_parser.h"
#include "files.h"
#include "typedef.h"

#define PORT_NUMBER 23000
#define MAX_PENDING_REQUEST 5
#define RCV_BUF_SIZE 1024
#define NUM_PREFORKED_PROCESS 5


void take_data(int sock_fd);
//void handler_on_die(void);

int main(){

    int fd_serv_sock ,fd_client_sock; /* socket fd */
    preforked_process pf_p[NUM_PREFORKED_PROCESS];
    struct sockaddr_in socket_address; /*struct with socket address info*/
    struct sockaddr_in client_address; /*struct with client address info*/
    unsigned int client_len; /*len of client's socket */
    int s_pipe[2];

    printf("Hello Sir, I'm alive!\n");

    if(socketpair(AF_UNIX, SOCK_DGRAM, 0, s_pipe) != 0)
        die_with_error("error opening socket_unix");



    fd_serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* creates the TSP socket IPv4 connection oriented*/

    if(fd_serv_sock == -1)
        die_with_error("error in sock()");

    memset((void *) &socket_address, 0, sizeof(socket_address)); /*void *memset(void *s, int c, size_t n)
                                                                  * fills  the  first  n  bytes of the memory area
                                                                    pointed to by s with the constant byte c. (man)*/

    socket_address.sin_family = AF_INET; /*IPv4 family*/
    socket_address.sin_port = htons(PORT_NUMBER); /*define port number (host-to-network byte order short --16 bit-- )*/
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY); /*server accepts request from any network interface
                                                         * (host-to-network byte order long --32 bit-- )*/

    if(bind(fd_serv_sock, (struct sockaddr *) &socket_address, sizeof(socket_address)) < 0) /*bind the socket with the port*/
    {
        //handler_on_die();
        die_with_error("error in bind()");
    }

    if(listen(fd_serv_sock, MAX_PENDING_REQUEST) <0) /*set the SO listening for connection without interrupt the server*/
    {
        //handler_on_die();
        die_with_error("error in listen()");
    }

    printf("I'm listening on port %d\n", PORT_NUMBER);

    printf("I'm ready!\n");

    /*Endless loop*/
    for(;;){

        client_len = sizeof(client_address);

        if((fd_client_sock = accept(fd_serv_sock,(struct sockaddr *) &client_address, &client_len)) < 0) /*accepts connection and creates a new socket (so a new fd) and set connection there*/
        {
            //handler_on_die();
            die_with_error("error in accept()");
        }

        printf("HANDLING CLIENT %s\n", inet_ntoa(client_address.sin_addr));

        take_data(fd_client_sock);
    }
}

void take_data(int sock_fd) {

    char buf[RCV_BUF_SIZE];
    char *url;
    int received;
    char file_size[256];

    received = recvn(sock_fd, buf, RCV_BUF_SIZE, 0);

    printf("%.*s\n", (int) strlen(buf), buf);
    if(received < 0) {
        //close_fd(sock_fd);
        //handler_on_die();
        die_with_error("error while receiving data");
    }
    printf("DATA RECEIVED\n\n"); /*--DEBUG--*/

    int len = received*sizeof(char);
    url = malloc(len);

    parse_data(url, buf, received);

    printf("%.*s\n",len, url);

    FILE *f;
    long file_length;

    f = open_file_read(url + 1);

    file_length = get_file_size(f);

    sprintf(file_size, "%ld", file_length);

    char ptr[file_length];

    read_file(f, ptr, file_length);

    close_file(f);

    sprintf(file_size, "Content-Length: %ld\r\n", file_length);
    sendn(sock_fd, "HTTP/1.1 200 OK\r\n", 0);
    sendn(sock_fd, file_size, 0);
    sendn(sock_fd, "Connection: Keep-Alive", 0);
    sendn(sock_fd, "Server: Caronte/Private\r\n\r\n", 0);
    sendn(sock_fd, ptr, 0);
    close_fd(sock_fd);
}

void handler_on_die(void){
    //close_fd(fd_serv_sock);
    fprintf(stderr, "I'm dieing, Sir! Goodbye!\n");
}