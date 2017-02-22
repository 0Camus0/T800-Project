#include "D3DXDriver.h"

#include <iostream>
#include <string>

#define DEBUG_DRIVER 1

ComPtr<IDXGISwapChain>			DXGISwapchain;
ComPtr<ID3D11Device>            D3D11Device;
ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;
ComPtr<ID3D11RenderTargetView>  D3D11RenderTargetView;

void D3DXDriver::InitDriver(){
	DXGI_MODE_DESC BackBufferDesc;
	ZeroMemory(&BackBufferDesc, sizeof(DXGI_MODE_DESC));
	BackBufferDesc.Width = Width;
	BackBufferDesc.Height = Height;
	BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BackBufferDesc.RefreshRate.Numerator = 0;
	BackBufferDesc.RefreshRate.Denominator = 1;
	BackBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BackBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	SwapChainDesc.BufferDesc = BackBufferDesc;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.OutputWindow = hwnd;
	SwapChainDesc.Windowed = false;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
#if DEBUG_DRIVER
		D3D11_CREATE_DEVICE_DEBUG,
#else
		0,
#endif
		NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &DXGISwapchain, &D3D11Device, NULL, &D3D11DeviceContext);


	ComPtr<ID3D11Texture2D> BackBuffer;
	hr = DXGISwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &BackBuffer);


	hr = D3D11Device->CreateRenderTargetView(BackBuffer.Get(), NULL, &D3D11RenderTargetView);


	D3D11DeviceContext->OMSetRenderTargets(1, D3D11RenderTargetView.GetAddressOf(), NULL);


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
	hwnd = GetActiveWindow();
}

void D3DXDriver::SetDimensions(int w, int h){
	Width=w;
	Height=h;
}

void D3DXDriver::Clear(){
	float rgba[4];
	rgba[0] = 0.0f;
	rgba[1] = 0.0f;
	rgba[2] = 0.0f;
	rgba[3] = 1.0f;

	D3D11DeviceContext->ClearRenderTargetView(D3D11RenderTargetView.Get(), rgba);
}

void D3DXDriver::SwapBuffers(){
	DXGISwapchain->Present(0, 0);
}