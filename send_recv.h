//
// Created by simone_mancini on 17/10/16.
//

#include <stdio.h>

#ifndef WEBSERVER_SEND_RECV_H
#define WEBSERVER_SEND_RECV_H

void sendn(int sock_fd, char *buf, int flags); /*return number of bytes sent*/

int recvn(int sock_fd, void *buf, size_t len, int flags); /*return number of bytes received*/

#endif //WEBSERVER_SEND_RECV_H
