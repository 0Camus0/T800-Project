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

#ifndef UAD_TEXTURE_GL_H
#define UAD_TEXTURE_GL_H

#include <Config.h>

#include <video\Texture.h>

class TextureGL : public Texture {
public:
	TextureGL() {}

	void	SetTextureParams(unsigned int &target);
	void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

	void	LoadAPITexture(unsigned char* buffer);
	void	LoadAPITextureCompressed(unsigned char* buffer);
};

#endif