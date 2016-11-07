//
// Created by Simone Mattogno on 31/10/2016.
//

#include <stdio.h>
#include <stdlib.h>

#include "tester.h"


static wurfl_t *wurfl;
const char* accept = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
char *image_extension = jpg;

/* It will be called by main thread
 *      @param accept: ok
 *      @param image_extension: I write get_filename_ext(char image_path)
 *                              that's called in image_info_init .... it is better to fix
 * */
float find_quality_factor(char *accept, char *image_extension) {


    char *token, *ptr, field[1024];
    float quality_factor = 1;

    strcat(field, "image/");
    strcat(field, image_extension);

    if ((ptr = strstr(accept, field)) != NULL) {
        token = ptr + strlen(field) + 1;
    } else if ((ptr = strstr(accept, "*/*")) != NULL) {
        token = ptr + strlen("*/*") + 1;
    } else return quality_factor;

    remove_spaces(token);
    if ((token = strstr(token, "q=")) != NULL) {
        token += strlen("q=");

        errno = 0;
        quality_factor = strtof(token, NULL);
        if (errno != 0) {
            perror("strtof");
            return -1;
        }
    }

    return quality_factor;
}


image_info_t *image_info_init(const char *base_path, float quality_factor)
{
    image_info_t *image_info = malloc(sizeof(image_info_t));
    if(image_info == NULL)
        die_with_error("Error allocating image_info structure");

    image_info->base_path = base_path;
    image_info->quality = quality_factor;
    const char *user_agent = rcv_msg -> user_agent;

    device_t * device = wurfl_match(wurfl, user_agent);

    image_info->max_width = atoi(device_capability(device, "max_image_width"));
    image_info->max_height = atoi(device_capability(device, "max_image_height"));
    image_info->colors = atoi(device_capability(device, "colors"));

    file_extension = get_filename_ext(image_path); //maybe it's better to do it before (?)

    image_info->extension = file_extension;

    return image_info;
}
