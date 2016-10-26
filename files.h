//
// Created by simone_mancini on 23/10/16.
//

#ifndef WEBSERVER_FILES_H
#define WEBSERVER_FILES_H

FILE *open_file_read(char *path);

long get_file_size(FILE *f);

void read_file(FILE *f, char *buf, long f_length);

void close_file(FILE *f);

void close_fd(int fd);

ssize_t write_unix_sock(int fd, void *ptr, size_t nbytes, int sendfd);
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

#endif //WEBSERVER_FILES_H
