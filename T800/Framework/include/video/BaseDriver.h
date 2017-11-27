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
    virtual void* GetAPIObject() const = 0;
    virtual void** GetAPIObjectReference() const = 0;

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
    virtual void* GetAPIObject() const = 0;
    virtual void** GetAPIObjectReference() const = 0;

    virtual void release() = 0;
    virtual Buffer* CreateBuffer(T8_BUFFER_TYPE::E bufferType, BufferDesc desc, void* initialData = nullptr) = 0;
    //virtual Shader* CreateShader(T8_SHADER_TYPE::E shaderType) = 0;
    //virtual Texture* CreateTexture(desc) = 0;
  };
  /* BUFFERS */
  class Buffer {
  public:
    virtual void* GetAPIObject() const = 0;
    virtual void** GetAPIObjectReference() const = 0;

    virtual void UpdateFromSystemCopy(const DeviceContext& deviceContext) = 0;
    virtual void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) = 0;
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

    virtual void	LoadAPITexture(DeviceContext* context, unsigned char* buffer) = 0;
    virtual void	LoadAPITextureCompressed(unsigned char* buffer) = 0;
    virtual void	DestroyAPITexture() = 0;

    virtual void	SetTextureParams() = 0;
    virtual void	GetFormatBpp(unsigned int &props, unsigned int &format, unsigned int &bpp) = 0;
    virtual void  Set(const DeviceContext& deviceContext, unsigned int slot, std::string shaderTextureName) = 0;
    virtual void  SetSampler(const DeviceContext& deviceContext) = 0;

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

  struct TechniqueProfile;
  class Technique {
  public:
    Technique();
    explicit Technique(std::string path);
    void Parse(std::string path);

    enum T8_TECHNIQUE_PROFILE{
      HLSL,
      GLES20,
      GLES30,
      GL
    };
    std::string m_name;
    std::vector<TechniqueProfile> m_profiles;

    std::vector<std::string> m_globalDefines;
  };
  struct TechniqueProfile {
    Technique::T8_TECHNIQUE_PROFILE m_type;
    std::string m_vsPath;
    std::string m_fsPath;
    std::string m_gsPath;
    std::string m_hsPath;
    std::string m_tsPath;

    std::vector<std::string> m_defines;
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
    GRAPHICS_API::E m_currentAPI;
  };



#ifndef GETDRIVERBASE
  extern BaseDriver *g_pBaseDriver;
#define GETDRIVERBASE() g_pBaseDriver
#endif

}

#endif
