//
// Created by Simone Mattogno on 31/10/2016.
//

#include <stdio.h>
#include <stdlib.h>

#include "tester.h"

static wurfl_t *wurfl;

char *file_path_extension(const char *file_path)
{
    uint64_t path_length = strlen(file_path);
    uint64_t extension_length = 0;
    for (char *pivot = (char *)file_path + path_length; extension_length < path_length; pivot--, extension_length++) {
        if (*pivot == '.')
        {
            extension_length--;
            pivot++;
            break;
        }
    }
    if (extension_length == 0) return "";

    char *buffer = malloc(extension_length + 1);
    memcpy(buffer, &file_path[path_length-extension_length], extension_length);
    buffer[extension_length] = '\0';
    return buffer;
}


image_info_t *image_info_init(const char *base_path, float quality) {

    image_info_t *image_info = malloc(sizeof(image_info_t));
    if (image_info == NULL)
        die_with_error("Error in malloc()");

    image_info->base_path = string_copy(base_path);
    image_info->quality = quality;
    return image_info;
}

http_response_t *http_response_image(const char *image_path) {

    float relative_quality_parameter = //TODO from http_request struct (function)

    image_info_t *image_info = image_init(image_path/*, relative_quality_parameter*/);

    const char *user_agent =//TODO from http_request struct

    device_t * device = wurfl_match(wurfl/*, user_agent*/);

    image_info->max_width = atoi(device_capability(device, "max_image_width"));
    image_info->max_height = atoi(device_capability(device, "max_image_height"));
    image_info->colors = atoi(device_capability(device, "colors"));

    char *preferred_file_extension = file_path_extension(image_path);

    char *preferred_file_extensions[] = {preferred_file_extension, "jpg", "png", "gif", NULL};
    for (char **iterator = preferred_file_extensions; *iterator != NULL; iterator++) {
        preferred_file_extension = *iterator;
        if (streq(device_capability(device, *iterator++), "true"))
            break;
    }

    image_info->extension = preferred_file_extension;

    return image_info;
}