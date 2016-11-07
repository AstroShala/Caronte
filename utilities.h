//
// Created by simone_mancini on 25/10/16.
//

#include <stdio.h>

#ifndef WEBSERVER_UTILITIES_H
#define WEBSERVER_UTILITIES_H

int max(int a, int b);

int min(int a, int b);

void remove_spaces(char *source);

char *get_filename_ext(char *filename);

void alloc_memory(void **ptr, size_t size);
#endif //WEBSERVER_UTILITIES_H
