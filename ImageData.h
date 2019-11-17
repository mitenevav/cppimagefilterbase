#pragma once
#include "stb_image.h"

struct image_data
{
	stbi_uc* pixels;
	int w, h;
	int compPerPixel;
};
