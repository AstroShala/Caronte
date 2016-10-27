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
    pthread_t tid;
    int sock_fd;
    char *url;
    int received;
    char file_size[256];
    char buf[1024];
} thread_data;

#endif //WEBSERVER_TYPEDEF_H
