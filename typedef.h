//
// Created by simone_mancini on 23/10/16.
//

#ifndef WEBSERVER_TYPEDEF_H
#define WEBSERVER_TYPEDEF_H

#include <unistd.h>

typedef struct{
    pid_t pid;
    int sock_fd;
    int *sock_pipe;
} preforked_process;

#endif //WEBSERVER_TYPEDEF_H
