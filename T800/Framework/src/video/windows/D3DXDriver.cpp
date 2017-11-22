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

#include <video/windows/D3DXDriver.h>
#include <video/windows/D3DXRT.h>
#include <video/windows/D3DXShader.h>

#include <iostream>
#include <string>



namespace t800 {
  // D3D11 Main Objects
  ComPtr<IDXGISwapChain>			DXGISwapchain;	// Responsible of the swap buffers
  ComPtr<ID3D11RenderTargetView>  D3D11RenderTargetView;  // View into the back buffer
  ComPtr<ID3D11DepthStencilView>  D3D11DepthStencilTargetView; // View into the depth buffer
  ComPtr<ID3D11Texture2D>			D3D11DepthTex;	// Actual depth buffer texture

  extern Device*            D3D11Device;
  extern DeviceContext*     D3D11DeviceContext;
  void ** D3DXDeviceContext::GetAPIContext() const
  {
    return (void**)&APIContext;
  }
  void D3DXDeviceContext::release()
  {
    APIContext->Release();
  }
  void D3DXDeviceContext::SetPrimitiveTopology(T8_TOPOLOGY::E topology)
  {
    D3D11_PRIMITIVE_TOPOLOGY apitopology;
    switch (topology)
    {
    case T8_TOPOLOGY::POINT_LIST:
      apitopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
      break;
    case T8_TOPOLOGY::LINE_LIST:
      apitopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
      break;
    case T8_TOPOLOGY::LINE_STRIP:
      apitopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
      break;
    case T8_TOPOLOGY::TRIANLE_LIST:
      apitopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
      break;
    case T8_TOPOLOGY::TRIANGLE_STRIP:
      apitopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
      break;
    default:
      break;
    }
    APIContext->IASetPrimitiveTopology(apitopology);
  }

  void D3DXDeviceContext::DrawIndexed(unsigned vertexCount, unsigned startIndex, unsigned startVertex)
  {
    APIContext->DrawIndexed(vertexCount, startIndex, startVertex);
  }


  void ** D3DXDevice::GetAPIDevice() const
  {
    return (void**)&APIDevice;
  }
  void D3DXDevice::release()
  {
    APIDevice->Release();
  }

  Buffer * D3DXDevice::CreateBuffer(T8_BUFFER_TYPE::E bufferType, BufferDesc desc, void* initialData)
  {
    Buffer* retBuff;
    switch (bufferType)
    {
    case T8_BUFFER_TYPE::VERTEX:
      retBuff = new D3DXVertexBuffer;
      break;
    case T8_BUFFER_TYPE::INDEX:
      retBuff = new D3DXIndexBuffer;
      break;
    case T8_BUFFER_TYPE::CONSTANT:
      retBuff = new D3DXConstantBuffer;
      break;
    default:
      break;
    }
    retBuff->Create(*this, desc, initialData);
    return retBuff;
  }


  void D3DXVertexBuffer::Set(const DeviceContext & deviceContext, const unsigned stride, const unsigned offset)
  {
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->IASetVertexBuffers(0, 1, &APIBuffer, &stride, &offset);
  }
  void ** D3DXVertexBuffer::GetAPIBuffer() const
  {
    return (void**)&APIBuffer;
  }
  void D3DXVertexBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    D3D11_USAGE usage;
    switch (desc.usage)
    {
    case T8_BUFFER_USAGE::DEFAULT:
      usage = D3D11_USAGE_DEFAULT;
      break;
    case T8_BUFFER_USAGE::DINAMIC:
      usage = D3D11_USAGE_DYNAMIC;
      break;
    case T8_BUFFER_USAGE::STATIC:
      usage = D3D11_USAGE_IMMUTABLE;
      break;
    default:
      usage = D3D11_USAGE_DEFAULT;
      break;
    }
    D3D11_BUFFER_DESC apiDesc{ 0 };
    apiDesc.ByteWidth = desc.byteWidth;
    apiDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    apiDesc.Usage = usage;
    //apiDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
    //apiDesc.StructureByteStride = ;
    //apiDesc.MiscFlags = ;

    if (initialData != nullptr)
    {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
      D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, &subData, &APIBuffer);
    }
    else
    {
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, 0, &APIBuffer);
    }

  }
  void D3DXVertexBuffer::UpdateFromSystemCopy(const DeviceContext& deviceContext)
  {
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, &sysMemCpy[0], 0, 0);
  }
  void D3DXVertexBuffer::UpdateFromBuffer(const DeviceContext& deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, buffer, 0, 0);
  }
  void D3DXVertexBuffer::release()
  {
    APIBuffer->Release();
    sysMemCpy.clear();
    delete this;
  }


  void D3DXIndexBuffer::Set(const DeviceContext & deviceContext, const unsigned offset, T8_IB_FORMAR::E format)
  {
    DXGI_FORMAT apiformat;
    if (format == T8_IB_FORMAR::R16)
      apiformat = DXGI_FORMAT::DXGI_FORMAT_R16_UINT;
    else
      apiformat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->IASetIndexBuffer(APIBuffer, apiformat, offset);
  }
  void ** D3DXIndexBuffer::GetAPIBuffer() const
  {
    return (void**)&APIBuffer;
  }
  void D3DXIndexBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    D3D11_USAGE usage;
    switch (desc.usage)
    {
    case T8_BUFFER_USAGE::DEFAULT:
      usage = D3D11_USAGE_DEFAULT;
      break;
    case T8_BUFFER_USAGE::DINAMIC:
      usage = D3D11_USAGE_DYNAMIC;
      break;
    case T8_BUFFER_USAGE::STATIC:
      usage = D3D11_USAGE_IMMUTABLE;
      break;
    default:
      usage = D3D11_USAGE_DEFAULT;
      break;
    }
    D3D11_BUFFER_DESC apiDesc{ 0 };
    apiDesc.ByteWidth = desc.byteWidth;
    apiDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    apiDesc.Usage = usage;

    if (initialData != nullptr)
    {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
      D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, &subData, &APIBuffer);
    }
    else
    {
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, 0, &APIBuffer);
    }
  }
  void D3DXIndexBuffer::UpdateFromSystemCopy(const DeviceContext& deviceContext)
  {
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, &sysMemCpy[0], 0, 0);
  }
  void D3DXIndexBuffer::UpdateFromBuffer(const DeviceContext& deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, buffer, 0, 0);
  }
  void D3DXIndexBuffer::release()
  {
    APIBuffer->Release();
    sysMemCpy.clear();
    delete this;
  }


  void D3DXConstantBuffer::Set(const DeviceContext & deviceContext)
  {
    ID3D11DeviceContext* context = reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext());
    context->VSSetConstantBuffers(0, 1, &APIBuffer);
    context->PSSetConstantBuffers(0, 1, &APIBuffer);
  }
  void ** D3DXConstantBuffer::GetAPIBuffer() const
  {
    return (void**)&APIBuffer;
  }
  void D3DXConstantBuffer::Create(const Device & device, BufferDesc desc, void * initialData)
  {
    descriptor = desc;
    ID3D11Device* apiDevice = reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice());
    D3D11_USAGE usage;
    switch (desc.usage)
    {
    case T8_BUFFER_USAGE::DEFAULT:
      usage = D3D11_USAGE_DEFAULT;
      break;
    case T8_BUFFER_USAGE::DINAMIC:
      usage = D3D11_USAGE_DYNAMIC;
      break;
    case T8_BUFFER_USAGE::STATIC:
      usage = D3D11_USAGE_IMMUTABLE;
      break;
    default:
      usage = D3D11_USAGE_DEFAULT;
      break;
    }
    D3D11_BUFFER_DESC apiDesc{ 0 };
    apiDesc.ByteWidth = desc.byteWidth;
    apiDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    apiDesc.Usage = usage;

    if (initialData != nullptr)
    {
      sysMemCpy.assign((char*)initialData, (char*)initialData + desc.byteWidth);
      D3D11_SUBRESOURCE_DATA subData = { initialData, 0, 0 };
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, &subData, &APIBuffer);
    }
    else
    {
      reinterpret_cast<ID3D11Device*>(*device.GetAPIDevice())->CreateBuffer(&apiDesc, 0, &APIBuffer);
    }
  }
  void D3DXConstantBuffer::UpdateFromSystemCopy(const DeviceContext & deviceContext)
  {
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, &sysMemCpy[0], 0, 0);
  }
  void D3DXConstantBuffer::UpdateFromBuffer(const DeviceContext & deviceContext, const void * buffer)
  {
    sysMemCpy.clear();
    sysMemCpy.assign((char*)buffer, (char*)buffer + descriptor.byteWidth);
    reinterpret_cast<ID3D11DeviceContext*>(*deviceContext.GetAPIContext())->UpdateSubresource(APIBuffer, 0, 0, (char*)buffer, 0, 0);
  }
  void D3DXConstantBuffer::release()
  {
    APIBuffer->Release();
    sysMemCpy.clear();
    delete this;
  }



  void D3DXDriver::InitDriver() {
    D3D11Device = new t800::D3DXDevice;
    D3D11DeviceContext = new t800::D3DXDeviceContext;

    //	Descriptor of the Back Buffer
    DXGI_MODE_DESC BackBufferDesc;
    ZeroMemory(&BackBufferDesc, sizeof(DXGI_MODE_DESC));
    BackBufferDesc.Width = Width;
    BackBufferDesc.Height = Height;
    BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 32 bit buffer
    BackBufferDesc.RefreshRate.Numerator = 0;
    BackBufferDesc.RefreshRate.Denominator = 1;
    BackBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    BackBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //	Descriptor of the Swap Chain
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    SwapChainDesc.BufferDesc = BackBufferDesc;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.OutputWindow = hwnd;
    SwapChainDesc.Windowed = true;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //	Function to create three objects:
    //	Device
    //	Device Context
    //	Swap Chain
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
#if DEBUG_DRIVER
      D3D11_CREATE_DEVICE_DEBUG,	// Debug errors
#else
      0,
#endif
      NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &DXGISwapchain,
      reinterpret_cast<ID3D11Device**>(D3D11Device->GetAPIDevice()), NULL,
      reinterpret_cast<ID3D11DeviceContext**>(D3D11DeviceContext->GetAPIContext()));

    ID3D11Device* device = reinterpret_cast<ID3D11Device*>(*D3D11Device->GetAPIDevice());
    ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());

    // Get the back buffer
    ComPtr<ID3D11Texture2D> BackBuffer;
    hr = DXGISwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer);

    // Descriptor to create the Depth Buffer
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = Width;
    descDepth.Height = Height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24 bits for depth 8 bits for stencil
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL; // -- > Use it as depth stencil
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = device->CreateTexture2D(&descDepth, NULL, &D3D11DepthTex);	// Output to the depth texture

    // Descriptor to create the Depth View
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // Using the View we can operate with the depth buffer, note this view is created from the depth texture
    hr = device->CreateDepthStencilView(D3D11DepthTex.Get(), &dsvd, &D3D11DepthStencilTargetView);


    //	Now we create the main render target view from the back buffer texture
    hr = device->CreateRenderTargetView(BackBuffer.Get(), NULL, &D3D11RenderTargetView);

    // Using the Context now we set the render targets, that would be the Main Render Target View (Back Buffer)
    // and the Depth Buffer View (Depth)
    deviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());


    // Set the Viewport of the size of the screen
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(Width);
    viewport.Height = static_cast<float>(Height);
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;

    deviceContext->RSSetViewports(1, &viewport);
  }

  void D3DXDriver::CreateSurfaces() {

  }

  void D3DXDriver::DestroySurfaces() {

  }

  void D3DXDriver::Update() {

  }

  void D3DXDriver::DestroyDriver() {
    D3D11Device->release();
    D3D11DeviceContext->release();
  }

  void D3DXDriver::SetWindow(void *window) {
    hwnd = GetActiveWindow(); // Get the HWND of the window
  }

  void D3DXDriver::SetDimensions(int w, int h) {
    Width = w;
    Height = h;
  }

  void D3DXDriver::Clear() {
    ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());
    float rgba[4];
    rgba[0] = 0.5f;
    rgba[1] = 0.5f;
    rgba[2] = 0.5f;
    rgba[3] = 1.0f;

    // Clearing the Main Render Target View
    deviceContext->ClearRenderTargetView(D3D11RenderTargetView.Get(), rgba);
    // Clearing the Depth Buffer
    deviceContext->ClearDepthStencilView(D3D11DepthStencilTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  void D3DXDriver::SwapBuffers() {
    // Swap between back and front buffer
    DXGISwapchain->Present(0, 0);
  }

  int  D3DXDriver::CreateTexture(std::string path) {
    D3DXTexture *pTex = new D3DXTexture;
    if (pTex->LoadTexture((char*)path.c_str())) {
      Textures.push_back(pTex);
      return (Textures.size() - 1);
    }
    else {
      delete pTex;
    }
    return -1;
  }

  Texture*  D3DXDriver::GetTexture(int id) {
    if (id < 0 || id >= (int)Textures.size()) {
      printf("Warning null ptr Textures Idx\n");
      return 0;
    }

    return Textures[id];
  }

  void  D3DXDriver::DestroyTexture() {
    for (unsigned int i = 0; i < Textures.size(); i++) {
      D3DXTexture *pTex = dynamic_cast<D3DXTexture*>(Textures[i]);
      delete pTex;
    }
  }

  int  D3DXDriver::CreateRT(int nrt, int cf, int df, int w, int h, bool GenMips) {
    D3DXRT	*pRT = new D3DXRT;
    if (w == 0)
      w = Width;
    if (h == 0)
      h = Height;
    pRT->number_RT = nrt;
    if (pRT->LoadRT(nrt, cf, df, w, h, GenMips)) {
      RTs.push_back(pRT);
      return (RTs.size() - 1);
    }
    else {
      delete pRT;
    }
    return -1;
  }

  void D3DXDriver::PushRT(int id) {
    ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());
    if (id < 0 || id >= (int)RTs.size()) {
      CurrentRT = -1;
      return;
    }

    CurrentRT = id;

    D3DXRT *pRT = dynamic_cast<D3DXRT*>(RTs[id]);

    std::vector<ID3D11RenderTargetView**> RTVA;
    for (int i = 0; i < pRT->number_RT; i++) {
      RTVA.push_back(pRT->vD3D11RenderTargetView[i].GetAddressOf());
    }

    if (pRT->number_RT == 0)
      RTVA.push_back(0);

    deviceContext->OMSetRenderTargets(pRT->number_RT, &RTVA[0][0], pRT->D3D11DepthStencilTargetView.Get());

    viewport_RT.TopLeftX = 0;
    viewport_RT.TopLeftY = 0;
    viewport_RT.Width = static_cast<float>(pRT->w);
    viewport_RT.Height = static_cast<float>(pRT->h);
    viewport_RT.MinDepth = 0;
    viewport_RT.MaxDepth = 1;

    deviceContext->RSSetViewports(1, &viewport_RT);

    float rgba[4];
    rgba[0] = 0.5f;
    rgba[1] = 0.5f;
    rgba[2] = 0.5f;
    rgba[3] = 1.0f;

    for (int i = 0; i < pRT->number_RT; i++) {
      deviceContext->ClearRenderTargetView(pRT->vD3D11RenderTargetView[i].Get(), rgba);
    }

    deviceContext->ClearDepthStencilView(pRT->D3D11DepthStencilTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  }

  void D3DXDriver::PopRT() {
    ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());
    deviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());


    deviceContext->RSSetViewports(1, &viewport);

    if (CurrentRT >= 0) {
      if (RTs[CurrentRT]->GenMips) {
        D3DXTexture* pTex = dynamic_cast<D3DXTexture*>(RTs[CurrentRT]->vColorTextures[0]);
        deviceContext->GenerateMips(pTex->pSRVTex.Get());
      }

    }

  }

  void D3DXDriver::DestroyRT(int id) {

  }

  void D3DXDriver::DestroyRTs() {
    for (unsigned int i = 0; i < RTs.size(); i++) {
      D3DXRT *pRT = dynamic_cast<D3DXRT*>(RTs[i]);
      delete pRT;
    }
  }

  Texture* D3DXDriver::GetRTTexture(int id, int index) {
    if (id < 0 || id >= (int)RTs.size())
      exit(666);

    if (index == DEPTH_ATTACHMENT) {
      return RTs[id]->pDepthTexture;
    }
    else {
      return RTs[id]->vColorTextures[index];
    }

  }

  int	D3DXDriver::CreateShader(std::string src_vs, std::string src_fs, unsigned int sig) {
    for (unsigned int i = 0; i < Shaders.size(); i++) {
      if (Shaders[i]->Sig == sig) {
        return i;
      }
    }

    D3DXShader* shader = new D3DXShader();
    if (shader->CreateShader(src_vs, src_fs, sig)) {
      Shaders.push_back(shader);
      return (Shaders.size() - 1);
    }
    else {
      delete shader;
    }
    return -1;

  }

  ShaderBase*	D3DXDriver::GetShaderSig(unsigned int sig) {
    for (unsigned int i = 0; i < Shaders.size(); i++) {
      if (Shaders[i]->Sig == sig) {
        return Shaders[i];
      }
    }
    printf("Warning null ptr ShaderBase Sig\n");
    return 0;
  }

  ShaderBase*	D3DXDriver::GetShaderIdx(int id) {
    if (id < 0 || id >= (int)Shaders.size()) {
      printf("Warning null ptr ShaderBase Idx\n");
      return 0;
    }

    return Shaders[id];
  }

  void D3DXDriver::DestroyShaders() {
    for (unsigned int i = 0; i < Shaders.size(); i++) {
      D3DXShader *pShader = dynamic_cast<D3DXShader*>(Shaders[i]);
      delete pShader;
    }
  }
}