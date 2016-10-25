//
// Created by simone_mancini on 07/10/16.
//

#include <stdio.h>
#include <stdlib.h>
#include "error_management.h"

void die_with_error(char *error_message){

    perror(error_message); /*print also the errno value */
    exit(EXIT_FAILURE);
}