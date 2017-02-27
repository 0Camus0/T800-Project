#ifndef UAD_TEXTURE_GL_H
#define UAD_TEXTURE_GL_H

#include "Config.h"

#include "Texture.h"

class TextureGL : public Texture {
public:
	TextureGL() {}

	void	SetTextureParams(unsigned int &target);
	void	GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp);

	void	LoadAPITexture(unsigned char* buffer);
	void	LoadAPITextureCompressed(unsigned char* buffer);
};

#endif