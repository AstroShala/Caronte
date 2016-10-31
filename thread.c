//
// Created by simone_mancini on 27/10/16.
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "thread.h"
#include "error_management.h"
#include "server_http_parser.h"
#include "files.h"
#include "send_recv.h"
#include "typedef.h"

void send_404_page_not_found(int sock_fd) {
    FILE *f;
    long file_length;
    char file_size[256];

    f = open_file_read("404pnf.html");
    file_length = get_file_size(f);

    sprintf(file_size, "%ld", file_length);

    char ptr[file_length];

    read_file(f, ptr, file_length);

    close_file(f);

    sprintf(file_size, "Content-Length: %ld\r\n", file_length);
    sendn(sock_fd, "HTTP/1.1 404 Not Found\r\n", 0);
    sendn(sock_fd, file_size, 0);
    sendn(sock_fd, "Connection: Keep-Alive", 0);
    sendn(sock_fd, "Server: Caronte/Private\r\n\r\n", 0);
    sendn(sock_fd, ptr, 0);

    //free(file_size);

}

void *thread_routine(void *arg){

    thread_data *td = (thread_data *) arg;

    printf("thread %ld alive\n", (long) td->tid);

    td->msg = malloc(sizeof(rcv_msg));

    if(td->msg == NULL)
        die_with_error("msg == NULL");

    parse_data(td->msg, td->buf, td->received);

    //printf("%.*s\n", len, url);
    printf("Host: %.*s\n", (int) td->msg->host_length, td->msg->host);
    printf("User-Agent: %.*s\n", (int) td->msg->user_agent_length, td->msg->user_agent);
    printf("Connection: %.*s\n", (int) td->msg->connection_length, td->msg->connection);
    printf("Referer: %.*s\n", (int) td->msg->referer_length, td->msg->referer);

    if(strcmp(td->msg->url, "/") == 0)  /*if request is "IP:PORT_NUMBER/" send index.html page*/
        td->msg->url = "/index.html";

    FILE *f;
    long file_length;

    f = open_file_read(td->msg->url + 1);

    if(f == NULL){
        send_404_page_not_found(td->sock_fd);
    }else {

        file_length = get_file_size(f);

        sprintf(td->file_size, "%ld", file_length);

        char ptr[file_length];

        read_file(f, ptr, file_length);

        close_file(f);

        sprintf(td->file_size, "Content-Length: %ld\r\n", file_length);
        sendn(td->sock_fd, "HTTP/1.1 200 OK\r\n", 0);
        sendn(td->sock_fd, td->file_size, 0);
        sendn(td->sock_fd, "Connection: Keep-Alive", 0);
        sendn(td->sock_fd, "Server: Caronte/Private\r\n\r\n", 0);
        sendn(td->sock_fd, ptr, 0);
    }

    free(td->msg->url);

    printf("thread %ld is dieing\n", (long) td->tid);
    return NULL;
}

void spawn_thread(thread_data *td){

    if(pthread_create(&td->tid, NULL, thread_routine, td) != 0)
        die_with_error("error in pthread_create");

}
