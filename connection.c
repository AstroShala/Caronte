//
// Created by simone_mancini on 07/10/16.
//

#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include "connection.h"
#include "error_management.h"

#define PORT_NUMBER 23000
#define MAX_PENDING_REQUEST 5

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

