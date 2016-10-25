//
// Created by simone_mancini on 07/10/16.
//

#include <sys/socket.h>

#ifndef WEBSERVER_CONNECTION_H
#define WEBSERVER_CONNECTION_H

int connect_retry_exponensial_backof(int sockfd, const struct sockaddr *address, socklen_t alen);

#endif //WEBSERVER_CONNECTION_H
