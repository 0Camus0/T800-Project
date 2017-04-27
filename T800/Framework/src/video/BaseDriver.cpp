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
#include <iostream>
#include <string>
#include <fstream>

BaseDriver*	g_pBaseDriver = 0;

#include <utils/Checker.h>

bool		Texture::LoadTexture(const char *fn) {
	bool found = false;
	std::string path = "Textures/";
	std::string filepath = path + std::string(fn);
	std::ifstream inf(filepath.c_str());
	found = inf.good();
	inf.close();

	int x = 0, y = 0, channels = 0;
	unsigned char *buffer = 0;

	if(!found){
		buffer = (unsigned char*)g_chkr.pixel_data;
		x = g_chkr.width;
		y = g_chkr.height;
		channels = g_chkr.bytes_per_pixel;
		std::cout << "Texture [" << filepath << "] not found, loading checker" << std::endl;
	}else{
		buffer = stbi_load(filepath.c_str(), &x, &y, &channels, 0);
#if FORCE_LOW_RES_TEXTURES
		if(buffer){

            int nx = x / FORCED_FACTOR;
            int ny = y / FORCED_FACTOR;

            unsigned char* resizedBuf = (unsigned char*)STBI_MALLOC(nx*ny*4 + 1);

            resizedBuf[nx*ny*4] = '\0';

            int result = stbir_resize_uint8(buffer,x,y,0,resizedBuf,nx,ny,0,4);

            stbi_image_free(buffer);

            buffer = resizedBuf;
            x = nx;
            y = ny;
            channels = 4;
        }
#endif
	}



	if (!buffer)
		return false;

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
	if(found){
		stbi_image_free(buffer);
	}

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

#if defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
	//	Defines += "#version 300 es\n\n";
#endif

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
	if (sig&Signature::GBUFF_PASS) {
		Defines += "#define G_BUFFER_PASS\n\n";
#if defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES31)
		Defines += "#define NON_LINEAR_DEPTH\n\n";
#endif
	}
	if (sig&Signature::FSQUAD_1_TEX)
		Defines += "#define FSQUAD_1_TEX\n\n";
	if (sig&Signature::FSQUAD_2_TEX)
		Defines += "#define FSQUAD_2_TEX\n\n";
	if (sig&Signature::FSQUAD_3_TEX)
		Defines += "#define FSQUAD_3_TEX\n\n";
	if (sig&Signature::DEFERRED_PASS) {
		Defines += "#define DEFERRED_PASS\n\n";
#if defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES31)
		Defines += "#define NON_LINEAR_DEPTH\n\n";
#endif
	}

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
