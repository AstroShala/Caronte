//
// Created by simone_mancini on 23/10/16.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "process_management.h"
#include "typedef.h"
#include "files.h"
#include "error_management.h"
#include "send_recv.h"
#include "server_http_parser.h"

#define RCV_BUF_SIZE 1024

void take_data(int sock_fd) {

    char buf[RCV_BUF_SIZE];
    char *url;
    int received;
    char file_size[256];

    for(;;) {
        memset(buf, 0, RCV_BUF_SIZE);
        memset(file_size, 0, sizeof(file_size));

        received = recvn(sock_fd, buf, RCV_BUF_SIZE, 0);

        printf("%.*s\n", (int) strlen(buf), buf);
        if (received == -1) {
            //close_fd(sock_fd);
            //handler_on_die();
            die_with_error("error while receiving data");
        }

        if (received == 0)
            break;
        printf("DATA RECEIVED\n\n"); /*--DEBUG--*/

        int len = received * sizeof(char);
        url = malloc(len);

        parse_data(url, buf, received);

        printf("%.*s\n", len, url);

        FILE *f;
        long file_length;

        f = open_file_read(url + 1);

        file_length = get_file_size(f);

        sprintf(file_size, "%ld", file_length);

        char ptr[file_length];

        read_file(f, ptr, file_length);

        close_file(f);

        sprintf(file_size, "Content-Length: %ld\r\n", file_length);
        sendn(sock_fd, "HTTP/1.1 200 OK\r\n", 0);
        sendn(sock_fd, file_size, 0);
        sendn(sock_fd, "Connection: Keep-Alive", 0);
        sendn(sock_fd, "Server: Caronte/Private\r\n\r\n", 0);
        sendn(sock_fd, ptr, 0);
    }
    close_fd(sock_fd);
}