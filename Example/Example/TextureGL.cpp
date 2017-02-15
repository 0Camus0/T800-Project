#include "TextureGL.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

void	TextureGL::SetTextureParams(unsigned int &params, unsigned int &target) {
	unsigned int glFiltering = 0;
	unsigned int glWrap = 0;
	
//		glFiltering = GL_NEAREST_MIPMAP_NEAREST;
//		glFiltering = GL_NEAREST_MIPMAP_LINEAR;
//		glFiltering = GL_LINEAR_MIPMAP_NEAREST;
//		glFiltering = GL_LINEAR_MIPMAP_LINEAR;
		glFiltering = GL_NEAREST;	
//		glFiltering = GL_LINEAR;
		
		glWrap = GL_CLAMP_TO_EDGE;	
//		glWrap = GL_REPEAT;

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, glFiltering);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, glFiltering);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, glWrap);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, glWrap);

	//int Max = 1;
	//glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Max);
	//glTexParameteri(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, Max);
}

void TextureGL::GetFormatBpp(unsigned int &props, unsigned int &glFormat, unsigned int &bpp) {

}

void TextureGL::LoadAPITexture(unsigned char* buffer) {
	unsigned int id;
	unsigned int glFormat = 0;
	unsigned int glChannel = GL_UNSIGNED_BYTE;
	unsigned int glTarget = GL_TEXTURE_2D;

	if (this->props&TEXT_BASIC_FORMAT::CH_ALPHA)
		glFormat = GL_ALPHA;
	else if (this->props&TEXT_BASIC_FORMAT::CH_RGB)
		glFormat = GL_RGB;
	else if (this->props&TEXT_BASIC_FORMAT::CH_RGBA)
		glFormat = GL_RGBA;

	glGenTextures(1, &id);
	glBindTexture(glTarget, id);

	if (this->x % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	else
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(glTarget, 0, glFormat, this->x, this->y, 0, glFormat, glChannel, (void*)(buffer));

	glGenerateMipmap(glTarget);

	SetTextureParams(this->params, glTarget);

	this->id = static_cast<unsigned short>(id);

}

void TextureGL::LoadAPITextureCompressed(unsigned char* buffer) {
	
}