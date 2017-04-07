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
#include <video/BaseDriver.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

BaseDriver*	g_pBaseDriver = 0;

int		Texture::LoadTexture(char *fn) {
	std::string path = "Textures/";
	path += std::string(fn);
	//path += "Checker.tga";
	int x = 0, y = 0, channels = 0;
	unsigned char *buffer = stbi_load(path.c_str(), &x, &y, &channels, 0);

	if (!buffer)
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
	return true;
}


bool BaseRT::LoadRT(int nrt, int cf, int df, int w, int h) {
	this->number_RT = nrt;
	this->color_format = cf;
	this->depth_format = df;
	this->w = w;
	this->h = h;
	return LoadAPIRT();
}

bool ShaderBase::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {
	std::string Defines = "";

	#if VDEBUG_NO_LIGHT
	Defines += "#define NO_LIGHT\n\n";
	#endif

	#if VDEBUG_SIMPLE_COLOR
	Defines += "#define SIMPLE_COLOR\n\n";
	#endif

	if (sig&Signature::HAS_NORMALS)
		Defines += "#define USE_NORMALS\n\n";
	if (sig&Signature::HAS_TEXCOORDS0)
		Defines += "#define USE_TEXCOORD0\n\n";
	if (sig&Signature::HAS_TEXCOORDS1)
		Defines += "#define USE_TEXCOORD1\n\n";
	if (sig&Signature::HAS_TANGENTS)
		Defines += "#define USE_TANGENTS\n\n";
	if (sig&Signature::HAS_BINORMALS)
		Defines += "#define USE_BINORMALS\n\n";
	if (sig&Signature::DIFFUSE_MAP)
		Defines += "#define DIFFUSE_MAP\n\n";
	if (sig&Signature::SPECULAR_MAP)
		Defines += "#define SPECULAR_MAP\n\n";
	if (sig&Signature::GLOSS_MAP)
		Defines += "#define GLOSS_MAP\n\n";
	if (sig&Signature::NORMAL_MAP)
		Defines += "#define NORMAL_MAP\n\n";
	if (sig&Signature::REFLECT_MAP)
		Defines += "#define REFLECT_MAP\n\n";
	if (sig&Signature::NO_LIGHT_AT_ALL)
		Defines += "#define NO_LIGHT\n\n";

	#ifdef USING_OPENGL
	Defines += "#define lowp \n\n";
	Defines += "#define mediump \n\n";
	Defines += "#define highp \n\n";
	#endif

	src_vs = Defines + src_vs;
	src_fs = Defines + src_fs;
	this->Sig = sig;
	return CreateShaderAPI(src_vs, src_fs, sig);
}