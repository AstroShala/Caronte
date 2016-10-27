//
// Created by simone_mancini on 23/10/16.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "files.h"
#include "error_management.h"

/* define if struct msghdr contains the msg_control element */
#define HAVE_MSGHDR_MSG_CONTROL 1

int error;

FILE *open_file_read(char *path){
    FILE *f;

    f = fopen(path, "r");

    //if(f == NULL)
        //die_with_error("error while opening file");

    return f;

}

long get_file_size(FILE *f){
    fseek(f, 0, SEEK_END);
    long file_length = ftell(f);
    if (file_length == -1) {
        perror("ftell");
    }
    rewind(f);
    return file_length;

}

void read_file(FILE *f, char *buf, long f_length){
    int result;

    result = fread(buf, 1, f_length, f);

    if(result != f_length)
        die_with_error("error in fread()");
}

void close_file(FILE *f){
    if(fclose(f) != 0)
        printf("error while closing file\n");
}

void close_fd(int fd){
    errno = 0;
    if(close(fd) == -1) {
        error = errno;

        if(error == EBADF){

        }else {
            printf("error while closing file\n");
        }
    }
}

ssize_t write_unix_sock(int fd, void *ptr, size_t nbytes, int sendfd){
    struct msghdr msg;
    struct iovec iov[1];

#ifdef  HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char    control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);

    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    *((int *) CMSG_DATA(cmptr)) = sendfd;
#endif

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    return (sendmsg(fd, &msg, 0));
}

ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd){
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;

#ifdef HAVE_MSGHDR_MSG_CONTROL
    union {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof (int))];
    } control_un;
    struct cmsghdr  *cmptr;

    msg.msg_control  = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
#endif

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    if ( (n = recvmsg(fd, &msg, 0)) <= 0)
        return (n);

#ifdef  HAVE_MSGHDR_MSG_CONTROL
    if ( (cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
        cmptr->cmsg_len == CMSG_LEN(sizeof(int))) {
        if (cmptr->cmsg_level != SOL_SOCKET)
            die_with_error("control level != SOL_SOCKET");
        if (cmptr->cmsg_type != SCM_RIGHTS)
            die_with_error("control type != SCM_RIGHTS");
        *recvfd = *CMSG_DATA(cmptr);
    } else
        *recvfd = -1;           /* descriptor was not passed */
#endif
    return (n);
}