//
// Created by Simone Mattogno on 31/10/2016.
//

#ifndef PROJECT_TESTER_H
#define PROJECT_TESTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "device.h"
#include "../constants.h"
#include "../error_management.h"

/*
 Initializes the structure for the information about the image
 @param base_path: path for the image
 @param quality: parameter for the quality requested
 @return: pointer to the image_info structure
 */
image_info_t *image_info_init(const char *base_path, float quality);

char *file_path_extension(const char *file_path);



#endif //PROJECT_TESTER_H
