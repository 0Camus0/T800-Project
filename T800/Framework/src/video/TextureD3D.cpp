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

#include <video\TextureD3D.h>

extern ComPtr<ID3D11Device>            D3D11Device;
extern ComPtr<ID3D11DeviceContext>     D3D11DeviceContext;

void	TextureD3D::SetTextureParams(unsigned int &target){
	D3D11_SAMPLER_DESC sdesc;

	sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sdesc.MaxAnisotropy = 1;

	if (params & TEXT_BASIC_PARAMS::MIPMAPS) {
		sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sdesc.MaxAnisotropy = 16;
	}

	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	if (params & TEXT_BASIC_PARAMS::CLAMP_TO_EDGE) {
		sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	if (params & TEXT_BASIC_PARAMS::TILED) {
		sdesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		sdesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		sdesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	}

	sdesc.BorderColor[0] = 0.0f;
	sdesc.BorderColor[1] = 0.0f;
	sdesc.BorderColor[2] = 0.0f;
	sdesc.BorderColor[3] = 0.0f;
	sdesc.MinLOD = 0.0f;
	sdesc.MaxLOD = 3.402823466e+38F;
	sdesc.MipLODBias = 0.0f;

	D3D11Device->CreateSamplerState(&sdesc, pSampler.GetAddressOf());

}

void	TextureD3D::GetFormatBpp(unsigned int &props, unsigned int &Format, unsigned int &bpp){

}

void	TextureD3D::LoadAPITexture(unsigned char* buffer){
	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = this->x;
	desc.Height = this->y;
	desc.ArraySize = 1;

	if (this->props&TEXT_BASIC_FORMAT::CH_ALPHA)
		desc.Format = DXGI_FORMAT_R8_UNORM;
	else 
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			
	desc.SampleDesc.Count = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = buffer;
	initData.SysMemPitch = sizeof(unsigned char) * this->x * 4;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = -1;
	
	HRESULT hr = D3D11Device->CreateTexture2D(&desc, nullptr, Tex.GetAddressOf());
	if(hr != S_OK){
		this->id = -1;
		return;
	}

	D3D11Device->CreateShaderResourceView(Tex.Get(), &srvDesc, pSRVTex.GetAddressOf());
	D3D11DeviceContext->UpdateSubresource(Tex.Get(), 0, 0, buffer, initData.SysMemPitch, 0);
	D3D11DeviceContext->GenerateMips(pSRVTex.Get());

	SetTextureParams(this->params);
	static int texid = 0;
	this->id = texid;
	texid++;
}

void	TextureD3D::LoadAPITextureCompressed(unsigned char* buffer){

}