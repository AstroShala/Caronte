//
// Created by simone_mancini on 23/10/16.
//

#ifndef WEBSERVER_TYPEDEF_H
#define WEBSERVER_TYPEDEF_H

#include <unistd.h>
#include <sys/types.h>

typedef struct{
    pid_t pid;         /* process ID */
    int pipe_fd;      /* parent's stream pipe to/from child */
    int status;      /* 0 = ready */
    long count;       /* number of connections handled */
} preforked_process;

typedef struct{
    char *field;
    char *url;
    char *host;
    char *user_agent;
    char *accept;
    char *accept_language;
    char *accept_encoding;
    char *referer;
    char *connection;
    size_t field_length;
    size_t url_length;
    size_t host_length;
    size_t user_agent_length;
    size_t accept_length;
    size_t accept_language_length;
    size_t accept_encoding_length;
    size_t referer_length;
    size_t connection_length;
} rcv_msg;

typedef struct{
    pthread_t tid;
    int sock_fd;
    rcv_msg *msg;
    int received;
    char file_size[256];
    char buf[1024];
} thread_data;

#endif //WEBSERVER_TYPEDEF_H
