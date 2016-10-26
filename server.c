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
#include <pthread.h>
#include <sys/wait.h>
#include "error_management.h"
#include "send_recv.h"
#include "server_http_parser.h"
#include "files.h"
#include "typedef.h"
#include "process_management.h"
#include "utilities.h"

#define PORT_NUMBER 23000
#define MAX_PENDING_REQUEST 5
#define RCV_BUF_SIZE 1024
#define NUM_PREFORKED_PROCESS 5

preforked_process pf_p[NUM_PREFORKED_PROCESS]; /* array of preforked_process structures*/

void take_data(int sock_fd);
void process_main();

void sigint_handler(int sig){

    int i;
    (void)sig;

    /*terminate all process*/
    for(i = 0; i < NUM_PREFORKED_PROCESS; i++) {
        kill(pf_p[i].pid, SIGTERM);
        printf("process %ld stopped\n", (long) pf_p[i].pid);
    }
    while(wait(NULL) > 0) /*wait for all children*/

    exit(EXIT_SUCCESS);
}

pid_t spown_preforked_process(int i, int serv_sock_fd){
    int sockfd[2];
    pid_t pid;

    socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);

    if ((pid = fork()) > 0) {
        close_fd(sockfd[1]);
        pf_p[i].pid = pid;          /*fill the struct preforked_process[i] */
        pf_p[i].pipe_fd = sockfd[0];
        pf_p[i].status = 0;
        return (pid);           /* parent */
    }

    dup2(sockfd[1], STDERR_FILENO); /* child's stream pipe to parent. It is set on stderr*/
    close_fd(sockfd[0]);
    close_fd(sockfd[1]);
    close_fd(serv_sock_fd);            /* child precess does not need this open */
    process_main();   /* never returns */
    return pid; /*never reached*/
}

void process_main(){
    char c;
    int client_fd;
    ssize_t n;

    printf("child %ld starting\n", (long) getpid());
    for(;;){
        if((n = read_fd(STDERR_FILENO, &c, 1, &client_fd)) == 0)
            die_with_error("error while reading client_fd");
        if(client_fd < 0)
            die_with_error("no descriptor passed from parent");

        if(setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, & (int) { 1 }, sizeof(int)) == -1)
            die_with_error("erorr while set socket option");

        take_data(client_fd); /*process request*/

        close_fd(client_fd);

        n = write(STDERR_FILENO, "" , 1); /*tell parent I'm ready again*/

        if(n == -1)
            die_with_error("error while writing on file");
    }
}


int tcp_listen(socklen_t *addr_len){

    int sock_fd;
    struct sockaddr_in socket_address; /*struct with socket address info*/

    sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* creates the TSP socket IPv4 connection oriented*/

    if(sock_fd == -1)
        die_with_error("error in sock()");

    memset((void *) &socket_address, 0, sizeof(socket_address)); /*void *memset(void *s, int c, size_t n)
                                                                  * fills  the  first  n  bytes of the memory area
                                                                    pointed to by s with the constant byte c. (man)*/

    socket_address.sin_family = AF_INET; /*IPv4 family*/
    socket_address.sin_port = htons(PORT_NUMBER); /*define port number (host-to-network byte order short --16 bit-- )*/
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY); /*server accepts request from any network interface
                                                         * (host-to-network byte order long --32 bit-- )*/

    *addr_len = sizeof(socket_address);

    if(bind(sock_fd, (struct sockaddr *) &socket_address, *addr_len) < 0) /*bind the socket with the port*/
    {
        //handler_on_die();
        die_with_error("error in bind()");
    }

    if(listen(sock_fd, MAX_PENDING_REQUEST) <0) /*set the SO listening for connection without interrupt the server*/
    {
        //handler_on_die();
        die_with_error("error in listen()");
    }

    return sock_fd;

}



int main(){

    int fd_serv_sock, fd_client_sock; /* socket fd */
    int i, maxfd, n_sel, rc;
    int childs_available; /*number of childs available*/
    ssize_t n;
    fd_set  rset, masterset;
    //struct sockaddr_in socket_address; /*struct with socket address info*/
    socklen_t addr_len, client_len;
    struct sockaddr_in *client_address; /*struct with client address info*/
    //int s_pipe[2];

    printf("Hello Sir, I'm alive!\n");

    fd_serv_sock = tcp_listen(&addr_len);

    FD_ZERO(&masterset); /*clears the fd set*/
    FD_SET(fd_serv_sock, &masterset); /*add fd_serv_sock to masterset*/

    maxfd = fd_serv_sock;

    client_address = malloc(addr_len);

    if(client_address == NULL)
        die_with_error("error in malloc()");

    childs_available = NUM_PREFORKED_PROCESS;

    /*prefork children*/
    for(i = 0; i < NUM_PREFORKED_PROCESS; i++) {
        spown_preforked_process(i, fd_serv_sock);
        FD_SET(pf_p[i].pipe_fd, &masterset);
        maxfd = max(maxfd, pf_p[i].pipe_fd);
    }

    if(signal(SIGINT, sigint_handler) == SIG_ERR)
        die_with_error("error in signal()");

    printf("I'm listening on port %d\n", PORT_NUMBER);

    printf("I'm ready!\n");

    /*Endless loop*/
    for(;;){
        rset = masterset;

        if(childs_available <= 0) /*turn of if no available children*/
            FD_CLR(fd_serv_sock, &rset);

        if((n_sel = select(maxfd + 1, &rset, NULL , NULL, NULL)) == -1) /*creates a three structure of file descriptor*/
            die_with_error("error in select()");

        /*check for new connection*/
        if(FD_ISSET(fd_serv_sock, &rset)){
            client_len = addr_len;

            if((fd_client_sock = accept(fd_serv_sock, (struct sockaddr *) client_address, &client_len)) == -1)
                die_with_error("error in accept");

            printf("HANDLING CLIENT");

            /*search for the first child process available*/
            for(i = 0; i < NUM_PREFORKED_PROCESS; i++)
                if(pf_p[i].status == 0)
                    break; /*available*/

            if(i == NUM_PREFORKED_PROCESS)
                die_with_error("no available children");

            pf_p[i].status = 1;
            pf_p[i].count++;
            childs_available--;

            n = write_unix_sock(pf_p[i].pipe_fd, "", 1, fd_client_sock);

            close_fd(fd_client_sock);

            if(--n_sel == 0)
                continue; /*all done with select() resulats*/
        }

        /*find any newly-available children*/
        for(i = 0; i < NUM_PREFORKED_PROCESS; i++){
            if(FD_ISSET(pf_p[i].pipe_fd, &rset)){
                if((n = read(pf_p[i].pipe_fd, &rc, 1)) == 0)
                    die_with_error("a child tarminated unexpectedly");

                pf_p[i].status = 0;
                childs_available++;

                if(--n_sel == 0)
                    break; /*all done with select() results*/
            }
        }
    }
}