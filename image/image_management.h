//
// Created by Simone Mattogno on 28/10/2016.
//

#ifndef PROJECT_IMAGE_MANAGEMENT_H
#define PROJECT_IMAGE_MANAGEMENT_H

#include "../utils/string.h"

#include <stdio.h>
#include <MagickWand/MagickWand.h>

/*
	Manipulates the image with magickwand functions and writes it into output path
	@param image_info: pointer to the image_info struct
	@param output: path where to write the output file
 */

int save_image_with_image_info(image_info_t *image_info, const char *output);

void image_dimensions_with_bounds(uint64_t real_dimensions[2], uint64_t bound_dimensions[2], uint64_t out_dimensions[2]);

#endif /* image_manipulation_h */

