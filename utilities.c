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

void alloc_memory(void **ptr, size_t size){

    *ptr = malloc(size);

    if(*ptr == NULL)
        die_with_error("error in malloc()");
}