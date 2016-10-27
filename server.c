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
#include <errno.h>
#include "error_management.h"
#include "send_recv.h"
#include "server_http_parser.h"
#include "files.h"
#include "typedef.h"
#include "process_management.h"
#include "utilities.h"
#include "connection.h"

#define PORT_NUMBER 23000
#define MAX_PENDING_REQUEST 5
#define RCV_BUF_SIZE 1024
#define NUM_PREFORKED_PROCESS 5

int error;
preforked_process pf_p[NUM_PREFORKED_PROCESS]; /* array of preforked_process structures*/

void take_data(int sock_fd);

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

    printf("I'm listening on port %d\n", PORT_NUMBER);

    printf("I'm ready!\n");

    FD_ZERO(&masterset); /*clears the fd set*/
    FD_SET(fd_serv_sock, &masterset); /*add fd_serv_sock to masterset*/

    maxfd = fd_serv_sock;

    client_address = malloc(addr_len);

    if(client_address == NULL)
        die_with_error("error in malloc()");

    childs_available = NUM_PREFORKED_PROCESS;

    /*prefork children*/
    for(i = 0; i < NUM_PREFORKED_PROCESS; i++) {
        spawn_preforked_process(i, fd_serv_sock, pf_p);
        FD_SET(pf_p[i].pipe_fd, &masterset);
        maxfd = max(maxfd, pf_p[i].pipe_fd);
    }

    if(signal(SIGINT, sigint_handler) == SIG_ERR)
        die_with_error("error in signal()");

    /*Endless loop*/
    for(;;){
        rset = masterset;

        if(childs_available <= 0) /*turn of if no available children*/
            FD_CLR(fd_serv_sock, &rset);

        if(!FD_ISSET(fd_serv_sock, &rset) && childs_available > 0)
            FD_SET(fd_serv_sock, &rset);

        errno = 0;
        if((n_sel = select(maxfd + 1, &rset, NULL , NULL, NULL)) == -1) { /*creates a three structure of file descriptor*/
            error = errno;
            if(error == EINTR){
                if((n_sel = select(maxfd + 1, &rset, NULL , NULL, NULL)) == -1)
                    die_with_error("error in select after signal received");
            }else {
                die_with_error("error in select()");
            }
        }

        /*check for new connection*/
        if(FD_ISSET(fd_serv_sock, &rset)){
            client_len = addr_len;

            errno = 0;
            if((fd_client_sock = accept(fd_serv_sock, (struct sockaddr *) client_address, &client_len)) == -1) {
                error = errno;
                if(error == EAGAIN || error == EWOULDBLOCK){
                    die_with_error("error in accept");
                }else if(error == ECONNABORTED && error == EINTR){
                    errno = 0;
                    if((fd_client_sock = accept(fd_serv_sock, (struct sockaddr *) client_address, &client_len)) == -1){
                        error = errno;
                        if(error == EINTR){
                            die_with_error("signal occurred while accept() execution");
                        }else if(error == ECONNABORTED){
                            die_with_error("connection down");
                        }else{
                            die_with_error("an error occurred while accept() execution");
                        }
                    }
                }else if(error == EINVAL){
                    fd_serv_sock = tcp_listen(&addr_len);
                    if((fd_client_sock = accept(fd_serv_sock, (struct sockaddr *) client_address, &client_len)) == -1)
                        die_with_error("an error occurred while accept() execution");
                }else{
                    die_with_error("an error occurred while accept() execution");
                }
            }

            printf("HANDLING CLIENT\n");

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
                continue; /*all done with select() results*/
        }

        /*find any newly-available children*/
        for(i = 0; i < NUM_PREFORKED_PROCESS; i++){
            if(FD_ISSET(pf_p[i].pipe_fd, &rset)){
                if((n = read(pf_p[i].pipe_fd, &rc, 1)) == 0) {
                    /*a child tarminated unexpectedly so create a new one*/
                    spawn_preforked_process(i, fd_serv_sock, pf_p);
                    FD_SET(pf_p[i].pipe_fd, &masterset);
                    maxfd = max(maxfd, pf_p[i].pipe_fd);
                }

                pf_p[i].status = 0;
                childs_available++;

                if(--n_sel == 0)
                    break; /*all done with select() results*/
            }
        }
    }
}