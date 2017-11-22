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

#ifndef T800_D3DXDRIVER_H
#define T800_D3DXDRIVER_H

#include <Config.h>

#include <video\BaseDriver.h>

#include <d3d11.h>
#include <dxgi.h>
#include <D3Dcompiler.h>

#include <wrl.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;


namespace t800 {
  /* DEVICES */
  class D3DXDeviceContext : public DeviceContext {
  public:
    void** GetAPIContext() const override;
    void release() override;
    void SetPrimitiveTopology(T8_TOPOLOGY::E topology) override;
    void DrawIndexed(unsigned vertexCount, unsigned startIndex, unsigned startVertex) override;
  private:
    ID3D11DeviceContext* APIContext;
  };
  class D3DXDevice : public Device {
  public:
    void** GetAPIDevice() const override;
    void release() override;
    Buffer* CreateBuffer(T8_BUFFER_TYPE::E bufferType, BufferDesc desc, void* initialData = nullptr) override;
  private:
    ID3D11Device* APIDevice;
  };

  /* BUFFERS */
  class D3DXVertexBuffer : public VertexBuffer {
  public:
    D3DXVertexBuffer() = default;
    D3DXVertexBuffer(D3DXVertexBuffer const& other) = default;
    D3DXVertexBuffer(D3DXVertexBuffer&& other) = default;

    void Set(const DeviceContext& deviceContext, const unsigned stride, const unsigned offset) override;
    void** GetAPIBuffer() const override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    ID3D11Buffer* APIBuffer;
  };
  class D3DXIndexBuffer : public IndexBuffer {
  public:
    void Set(const DeviceContext& deviceContext, const unsigned offset, T8_IB_FORMAR::E format = T8_IB_FORMAR::R32) override;
    void** GetAPIBuffer() const override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    ID3D11Buffer* APIBuffer;
  };
  class D3DXConstantBuffer : public ConstantBuffer {
  public:
    void Set(const DeviceContext& deviceContext) override;
    void** GetAPIBuffer() const override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    ID3D11Buffer* APIBuffer;
  };



  class D3DXDriver : public BaseDriver {
  public:
    D3DXDriver() { m_currentAPI = GRAPHICS_API::D3D11; }
    void	InitDriver();
    void	CreateSurfaces();
    void	DestroySurfaces();
    void	Update();
    void	DestroyDriver();
    void	SetWindow(void *window);
    void	SetDimensions(int, int);

    int 	 CreateTexture(std::string);
    Texture* GetTexture(int id);
    void	 DestroyTexture();

    int 	 CreateRT(int nrt, int cf, int df, int w, int h, bool GenMips = false);
    void	 PushRT(int id);
    void	 PopRT();
    void     DestroyRT(int id);
    void	 DestroyRTs();
    Texture* GetRTTexture(int id, int index);

    int			CreateShader(std::string src_vs, std::string src_fs, unsigned int sig);
    ShaderBase*	GetShaderSig(unsigned int sig);
    ShaderBase*	GetShaderIdx(int Id);
    void		DestroyShaders();

    void	Clear();
    void	SwapBuffers();

    int		Width;
    int		Height;
    HWND	hwnd;

    D3D11_VIEWPORT viewport;
    D3D11_VIEWPORT viewport_RT;
  };
}

#endif