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

#ifndef T800_BASEDRIVER_H
#define T800_BASEDRIVER_H

#include <Config.h>
#include <string>
#include <vector>


enum TEXT_BASIC_FORMAT {
	CH_ALPHA = 1,
	CH_RGB = 2,
	CH_RGBA = 4
};

enum TEXT_BASIC_PARAMS {
	TILED = 1,
	CLAMP_TO_EDGE = 2,
	MIPMAPS = 4
};

class Texture {
public:
	Texture() :
		size(0),
		props(0),
		params(0),
		x(0),
		y(0),
		id(0),
		bounded(0),
		mipmaps(0)
	{

	}

    virtual ~Texture(){}

	bool			LoadTexture(const char *fn);

	virtual void	LoadAPITexture(unsigned char* buffer) = 0;
	virtual void	LoadAPITextureCompressed(unsigned char* buffer) = 0;

	virtual void	SetTextureParams() = 0;
	virtual void	GetFormatBpp(unsigned int &props, unsigned int &format, unsigned int &bpp) = 0;

	char			optname[128];
	unsigned int	size;
	unsigned int	props;
	unsigned int	params;
	unsigned short	x, y;
	unsigned int	id;
	unsigned char	bounded;
	unsigned char	mipmaps;
};

struct BaseRT {
	enum ATTACHMENTS{
		COLOR0_ATTACHMENT = 1,
		COLOR1_ATTACHMENT = 2,
		COLOR2_ATTACHMENT = 4,
		COLOR3_ATTACHMENT = 8,
		DEPTH_ATTACHMENT  = 16
	};

	enum FORMAT {
		FD16 = 0,
		F32,
		RGB8,
		RGBA8,
		RGBA16F,
		RGBA32F,
		BGR8,
		BGRA8,
		BGRA32
	};

	bool			LoadRT(int nrt, int cf, int df, int w, int h);
	virtual bool	LoadAPIRT() = 0;

	int w;
	int h;
	int number_RT;
	int color_format;
	int depth_format;

	std::vector<Texture*>							vColorTextures;
	Texture*										pDepthTexture;
};

enum Signature {
	FORWARD_PASS = 0,
	// MAPS
	DIFFUSE_MAP = 1,
	SPECULAR_MAP = 2,
	GLOSS_MAP = 4,
	NORMAL_MAP = 8,
	REFLECT_MAP = 16,
	// ATTRIBUTES
	HAS_NORMALS = 32,
	HAS_TANGENTS = 64,
	HAS_BINORMALS = 128,
	HAS_TEXCOORDS0 = 256,
	HAS_TEXCOORDS1 = 512,
	// CASES
	NO_LIGHT_AT_ALL = 1024,
	// PASSES
	GBUFF_PASS		= 2048,
	SHADOW_MAP_PASS = 4096,
	FSQUAD_1_TEX	= 8192,
	FSQUAD_2_TEX	= 16384,
	FSQUAD_3_TEX	= 32768,
	DEFERRED_PASS	= 65536
};

class ShaderBase {
public:
	ShaderBase() : Sig(0) {	}
	bool			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig);
	virtual bool    CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) = 0;

	unsigned int	Sig;
};

class BaseDriver {
public:
	BaseDriver() {  }
	virtual	void	 InitDriver() = 0;
	virtual void	 CreateSurfaces() = 0;
	virtual void	 DestroySurfaces() = 0;
	virtual void	 Update() = 0;
	virtual void	 DestroyDriver() = 0;
	virtual void	 SetWindow(void *window) = 0;
	virtual void	 SetDimensions(int,int) = 0;
	virtual void	 Clear() = 0;
	virtual void	 SwapBuffers() = 0;

	virtual int 	 CreateTexture(std::string) = 0;
	virtual Texture* GetTexture(int id) = 0;
	virtual void	 DestroyTexture() = 0;

	virtual int 	 CreateRT(int nrt, int cf, int df,int w, int h) = 0;
	virtual void	 PushRT(int id) = 0;
	virtual void	 PopRT() = 0;
	virtual void	 DestroyRTs() = 0;

	virtual int			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) = 0;
	virtual ShaderBase*	GetShaderSig(unsigned int sig) = 0;
	virtual ShaderBase*	GetShaderIdx(int id) = 0;
	virtual void		DestroyShaders() = 0;

	std::vector<ShaderBase*>	Shaders;
	std::vector<BaseRT*>		RTs;
	std::vector<Texture*>		Textures;
};



#ifndef GETDRIVERBASE
extern BaseDriver *g_pBaseDriver;
#define GETDRIVERBASE() g_pBaseDriver
#endif

#endif
