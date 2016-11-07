//
// Created by simone_mancini on 25/10/16.
//

#include <stdlib.h>
#include "utilities.h"
#include "error_management.h"

int max(int a, int b){

    if(a > b)
        return a;

    return b;
}


int min(int a, int b){

    if(a < b)
        return a;

    return b;
}


void remove_spaces(char *source) {
    char *i = source;
    char *j = source;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ')
            i++;
    }
    *i = 0;
}


char *get_filename_ext(char *filename) {

    char *dot = strrchr(filename, '.'); //last occurrence
    if (!dot || dot == filename) return "";
    return dot + 1;
}



void alloc_memory(void **ptr, size_t size){

    *ptr = malloc(size);

    if(*ptr == NULL)
        die_with_error("error in malloc()");
}