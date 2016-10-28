//
// Created by Simone Mattogno on 28/10/2016.
//

#ifndef string_h
#define string_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <fcntl.h>
#include "string.h"
#include <ctype.h>
#include <errno.h>
#include <time.h>

int intlen(int value);

char *str(int value);

ssize_t fd_getline(int fd, char **out);

int index_of_char(const char *string, const char character);

char *substr(const char *string, const int start_index, const int end_index);

char *string_by_appending_string(const char *base_string, const char *string_to_append);

char *string_by_adding_path_component(const char *base_path, const char *path_component);

char *file_path_extension(const char *file_path);

char *rand_string(char *str, size_t size);

int streq(const char *a, const char *b);

void urldecode2(char *dst, const char *src);

const char *string_copy(const char *string);

void formatted_time(char *buffer, size_t size, const char *format);

#endif /* string_h */
