//
// Created by simone_mancini on 23/10/16.
//

#include <stdio.h>
#include <unistd.h>
#include "files.h"
#include "error_management.h"

FILE *open_file_read(char *path){
    FILE *f;

    f = fopen(path, "r");

    if(f == NULL)
        die_with_error("error while opening file");

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
    if(close(fd) != 0)
        printf("error while closing file\n");

    printf("socket %d closed", fd);
}