/*********************************************************
* Copyright (C) 2017 Daniel Enriquez (camus_mm@hotmail.com)
* All Rights Reserved
*
* You may use, distribute and modify this code under the
* following terms:
* ** Do not claim that you wrote this software
* ** A mention would be appreciated but not needed
* ** I do not and will not provide support, this software is "as is"
* ** Enjoy, learn and share.
*********************************************************/

#include <video\Texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

int		Texture::LoadTexture(char *fn){
	std::string path = "Textures/";
	path += std::string(fn);
	//path += "Checker.tga";
	int x = 0, y = 0, channels = 0;
	unsigned char *buffer = stbi_load(path.c_str(), &x, &y, &channels, 0);

	if(!buffer)
		return -1;

	size = x*y*channels;
	bounded = 1;
	this->x = x;
	this->y = y;
	mipmaps = 1;
	this->params = params;

	switch (channels) {
		case 1: {
			props |= TEXT_BASIC_FORMAT::CH_ALPHA;
		}break;
		case 3: {
			props |= TEXT_BASIC_FORMAT::CH_RGB;
		}break;
		case 4: {
			props |= TEXT_BASIC_FORMAT::CH_RGBA;
		}break;
	}

	memcpy(&optname[0], fn, strlen(fn));
	optname[strlen(fn)] = '\0';

	LoadAPITexture(buffer);
	stbi_image_free(buffer);
	return id;
}
