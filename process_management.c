//
// Created by simone_mancini on 23/10/16.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "process_management.h"
#include "typedef.h"
#include "files.h"
#include "error_management.h"
#include "send_recv.h"
#include "server_http_parser.h"
#include "utilities.h"
#include "thread.h"

#define RCV_BUF_SIZE 1024

void send_404_page_not_found(int sock_fd);

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
            die_with_error("error while set socket option");

        take_data(client_fd); /*process request*/

        printf("--CLIENT SERVED--\n"); /*DEBUG*/

        close_fd(client_fd);

        n = write(STDERR_FILENO, "" , 1); /*tell parent I'm ready again*/

        if(n == -1)
            die_with_error("error while writing on file");
    }
}

pid_t spawn_preforked_process(int i, int serv_sock_fd, preforked_process *pf_p){
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

void take_data(int sock_fd) {

    char buf[RCV_BUF_SIZE];
    int received;
    char file_size[256];
    thread_data *td;

    for(;;) {
        memset(buf, 0, RCV_BUF_SIZE);
        memset(file_size, 0, sizeof(file_size));

        received = recvn(sock_fd, buf, RCV_BUF_SIZE, 0);

        printf("%.*s\n", (int) strlen(buf), buf);
        if (received == -1) {
            die_with_error("error while receiving data");
        }

        printf("%d\n", received);

        if (received == EOF)
            break;

        //printf("DATA RECEIVED\n\n"); /*--DEBUG--*/

        int len = received * sizeof(char);
        //url = malloc(len);

        alloc_memory((void *) &td, sizeof(thread_data));

        td->sock_fd = sock_fd;
        strcpy(td->buf, buf);
        td->received = received;

        spawn_thread(td);

        pthread_join(td->tid, NULL);

        break;
    }
}