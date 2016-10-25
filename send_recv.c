//
// Created by simone_mancini on 17/10/16.
//

#include <sys/socket.h>
#include <string.h>
#include "send_recv.h"
#include "error_management.h"

/*writes n bytes on socket and sends them*/
void sendn(int sock_fd, char *buf, int flags) {

    int len = strlen(buf);

    if (send(sock_fd, buf, len, flags) == -1)
        die_with_error("send() sent a different number of bytes than expected");

}

/*reads n bytes from socket*/
int recvn(int sock_fd, void *buf, size_t len, int flags){

    int rcvb;

    if((rcvb = recv(sock_fd, buf, len, flags)) < 0)
        die_with_error("recv() failed or connection closed prematurally");

    return rcvb;
}
