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

#include <utils/cil.h>
#include <Config.h>
#include <string>
#include <vector>
#include "T8_descriptors.h"


class Shader;
namespace t800 {
  class Buffer;
  class VertexBuffer;
  class IndexBuffer;
  class ConstantBuffer;
  class DeviceContext {
  public:
    virtual void** GetAPIContext() const = 0;
    virtual void release() = 0;
    virtual void SetPrimitiveTopology(T8_TOPOLOGY::E topology) = 0;
    virtual void DrawIndexed(unsigned vertexCount, unsigned startIndex, unsigned startVertex) = 0;

    ConstantBuffer* actualConstantBuffer;
    IndexBuffer* actualIndexBuffer;
    VertexBuffer* actualVertexBuffer;
    Shader* actualShaderSet;
  };
  class Device {
  public:
    virtual void** GetAPIDevice() const = 0;
    virtual void release() = 0;
    virtual Buffer* CreateBuffer(T8_BUFFER_TYPE::E bufferType, BufferDesc desc, void* initialData = nullptr) = 0;
    //virtual Shader* CreateShader(T8_SHADER_TYPE::E bufferType) = 0;
  };
  /* BUFFERS */
  class Buffer {
  public:
    virtual void UpdateFromSystemCopy(const DeviceContext& deviceContext) = 0;
    virtual void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) = 0;
    virtual void** GetAPIBuffer() const = 0;
    virtual void release() = 0;
    virtual void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) = 0;

    BufferDesc descriptor;
    std::vector<char> sysMemCpy;
  protected:
  };
  class VertexBuffer : public Buffer {
  public:
    virtual void Set(const DeviceContext& deviceContext, const unsigned stride, const unsigned offset) = 0;
  };
  class IndexBuffer : public Buffer {
  public:
    virtual void Set(const DeviceContext& deviceContext, const unsigned offset, T8_IB_FORMAR::E format = T8_IB_FORMAR::R32) = 0;
  };
  class ConstantBuffer : public Buffer {
  public:
    virtual void Set(const DeviceContext& deviceContext) = 0;
  };

  /* BUFFERS */

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

    virtual ~Texture() {}

    bool			LoadTexture(const char *fn);
    void			DestroyTex();

    virtual void	LoadAPITexture(t800::DeviceContext* context, unsigned char* buffer) = 0;
    virtual void	LoadAPITextureCompressed(unsigned char* buffer) = 0;
    virtual void	DestroyAPITexture() = 0;

    virtual void	SetTextureParams() = 0;
    virtual void	GetFormatBpp(unsigned int &props, unsigned int &format, unsigned int &bpp) = 0;
    virtual void  Set(const t800::DeviceContext& deviceContext, unsigned int slot, std::string shaderTextureName) = 0;
    virtual void  SetSampler(const t800::DeviceContext& deviceContext) = 0;

    char			optname[128];
    unsigned int	size;
    unsigned int	props;
    unsigned int	params;
    unsigned int	cil_props;
    unsigned int	x, y;
    unsigned int	id;
    unsigned int	bounded;
    unsigned int	mipmaps;
    std::string m_shaderTextureName;
  };

  struct BaseRT {
    enum ATTACHMENTS {
      COLOR0_ATTACHMENT = 1,
      COLOR1_ATTACHMENT = 2,
      COLOR2_ATTACHMENT = 4,
      COLOR3_ATTACHMENT = 8,
      DEPTH_ATTACHMENT = 16
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
      BGRA32,
      NOTHING
    };

    bool			LoadRT(int nrt, int cf, int df, int w, int h, bool GenMips = false);
    virtual bool	LoadAPIRT() = 0;

    void			DestroyRT();
    virtual void	DestroyAPIRT() = 0;

    int w;
    int h;
    int number_RT;
    int color_format;
    int depth_format;
    bool GenMips;

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
    GBUFF_PASS = 2048,
    SHADOW_MAP_PASS = 4096,
    FSQUAD_1_TEX = 8192,
    FSQUAD_2_TEX = 16384,
    FSQUAD_3_TEX = 32768,
    DEFERRED_PASS = 65536,
    SHADOW_COMP_PASS = 131072,
    VERTICAL_BLUR_PASS = 262144,
    HORIZONTAL_BLUR_PASS = 524288,
    ONE_PASS_BLUR = 1048576,
    BRIGHT_PASS = 2097152,
    HDR_COMP_PASS = 4194304
  };

  class ShaderBase {
  public:
    ShaderBase() : Sig(0) {	}
    bool			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig);
    virtual bool    CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned int sig) = 0;
    virtual void  Set(const t800::DeviceContext& deviceContext) = 0;

    unsigned int	Sig;
  };

  class BaseDriver {
  public:
    enum {
      DEPTH_ATTACHMENT = -1,
      COLOR0_ATTACHMENT = 0,
      COLOR1_ATTACHMENT = 1,
      COLOR2_ATTACHMENT = 2,
      COLOR3_ATTACHMENT = 3,
      COLOR4_ATTACHMENT = 4,
      COLOR5_ATTACHMENT = 5,
      COLOR6_ATTACHMENT = 6,
      COLOR7_ATTACHMENT = 7,
    };
    BaseDriver() : CurrentRT(-1) {  }
    virtual	void	 InitDriver() = 0;
    virtual void	 CreateSurfaces() = 0;
    virtual void	 DestroySurfaces() = 0;
    virtual void	 Update() = 0;
    virtual void	 DestroyDriver() = 0;
    virtual void	 SetWindow(void *window) = 0;
    virtual void	 SetDimensions(int, int) = 0;
    virtual void	 Clear() = 0;
    virtual void	 SwapBuffers() = 0;

    virtual int 	 CreateTexture(std::string) = 0;
    virtual Texture* GetTexture(int id) = 0;
    virtual void	 DestroyTexture() = 0;

    virtual int 	 CreateRT(int nrt, int cf, int df, int w, int h, bool genMips = false) = 0;
    virtual void	 PushRT(int id) = 0;
    virtual void	 PopRT() = 0;
    virtual void	 DestroyRT(int id) = 0;
    virtual void	 DestroyRTs() = 0;
    virtual Texture* GetRTTexture(int id, int index) = 0;

    virtual int			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) = 0;
    virtual ShaderBase*	GetShaderSig(unsigned int sig) = 0;
    virtual ShaderBase*	GetShaderIdx(int id) = 0;
    virtual void		DestroyShaders() = 0;

    std::vector<ShaderBase*>	Shaders;
    std::vector<BaseRT*>		RTs;
    std::vector<Texture*>		Textures;
    int							CurrentRT;
  };



#ifndef GETDRIVERBASE
  extern BaseDriver *g_pBaseDriver;
#define GETDRIVERBASE() g_pBaseDriver
#endif

}

#endif
