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

#include <video\D3DXDriver.h>
#include <video\D3DXRT.h>
#include <video\D3DXShader.h>

#include <iostream>
#include <string>

#define DEBUG_DRIVER 0

// D3D11 Main Objects
ComPtr<IDXGISwapChain>			DXGISwapchain;	// Responsible of the swap buffers
ComPtr<ID3D11Device>            D3D11Device;	// Device for create resources
ComPtr<ID3D11DeviceContext>     D3D11DeviceContext; // Context to set and manipulate the resources
ComPtr<ID3D11RenderTargetView>  D3D11RenderTargetView;  // View into the back buffer
ComPtr<ID3D11DepthStencilView>  D3D11DepthStencilTargetView; // View into the depth buffer
ComPtr<ID3D11Texture2D>			D3D11DepthTex;	// Actual depth buffer texture

void D3DXDriver::InitDriver(){
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
		NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &DXGISwapchain, &D3D11Device, NULL, &D3D11DeviceContext);

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
	hr = D3D11Device->CreateTexture2D(&descDepth, NULL, &D3D11DepthTex);	// Output to the depth texture

	// Descriptor to create the Depth View
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));

	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// Using the View we can operate with the depth buffer, note this view is created from the depth texture
	hr = D3D11Device->CreateDepthStencilView(D3D11DepthTex.Get(), &dsvd, &D3D11DepthStencilTargetView);


	//	Now we create the main render target view from the back buffer texture
	hr = D3D11Device->CreateRenderTargetView(BackBuffer.Get(), NULL, &D3D11RenderTargetView);

	// Using the Context now we set the render targets, that would be the Main Render Target View (Back Buffer)
	// and the Depth Buffer View (Depth)
	D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());


	// Set the Viewport of the size of the screen
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(Width);
	viewport.Height = static_cast<float>(Height);
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	D3D11DeviceContext->RSSetViewports(1, &viewport);
}

void D3DXDriver::CreateSurfaces(){

}

void D3DXDriver::DestroySurfaces(){

}

void D3DXDriver::Update(){

}

void D3DXDriver::DestroyDriver(){

}

void D3DXDriver::SetWindow(void *window){
	hwnd = GetActiveWindow(); // Get the HWND of the window 
}

void D3DXDriver::SetDimensions(int w, int h){
	Width=w;
	Height=h;
}

void D3DXDriver::Clear(){
	float rgba[4];
	rgba[0] = 0.5f;
	rgba[1] = 0.5f;
	rgba[2] = 0.5f;
	rgba[3] = 1.0f;

	// Clearing the Main Render Target View
	D3D11DeviceContext->ClearRenderTargetView(D3D11RenderTargetView.Get(), rgba);
	// Clearing the Depth Buffer
	D3D11DeviceContext->ClearDepthStencilView(D3D11DepthStencilTargetView.Get(), D3D11_CLEAR_DEPTH , 1.0f, 0);
}

void D3DXDriver::SwapBuffers(){
	// Swap between back and front buffer
	DXGISwapchain->Present(0, 0);
}

int  D3DXDriver::CreateRT(int nrt, int cf, int df, int w, int h) {
	D3DXRT	*pRT = new D3DXRT;
	if (w == 0)
		w = Width;
	if (h == 0)
		h = Height;
	pRT->number_RT = nrt;
	if (pRT->LoadRT(nrt, cf, df, w, h)) {
		RTs.push_back(pRT);
		return (RTs.size() - 1);
	}
	else {
		delete pRT;
	}
	return -1;
}

void D3DXDriver::PushRT(int id) {
	if (id < 0 || id >= (int)RTs.size())
		return;

	D3DXRT *pRT = dynamic_cast<D3DXRT*>(RTs[id]);
	
	std::vector<ID3D11RenderTargetView**> RTVA;
	for (int i = 0; i < pRT->number_RT; i++) {
		RTVA.push_back(pRT->vD3D11RenderTargetView[i].GetAddressOf());
	}

	D3D11DeviceContext->OMSetRenderTargets(pRT->number_RT, &RTVA[0][0], pRT->D3D11DepthStencilTargetView.Get());
	
	float rgba[4];
	rgba[0] = 0.5f;
	rgba[1] = 0.5f;
	rgba[2] = 0.5f;
	rgba[3] = 1.0f;

	for (int i = 0; i < pRT->number_RT; i++) {
		D3D11DeviceContext->ClearRenderTargetView(pRT->vD3D11RenderTargetView[i].Get(), rgba);
	}
	
	D3D11DeviceContext->ClearDepthStencilView(pRT->D3D11DepthStencilTargetView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void D3DXDriver::PopRT() {
	D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), D3D11DepthStencilTargetView.Get());
}

void D3DXDriver::DestroyRTs() {
	for (unsigned int i = 0; i < RTs.size(); i++) {
		D3DXRT *pRT = dynamic_cast<D3DXRT*>(RTs[i]);
		delete pRT;
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