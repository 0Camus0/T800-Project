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

#include <video/windows/D3DXShader.h>
#include <scene/windows/D3DXQuad.h>
#include <utils/Utils.h>

extern t800::Device*            D3D11Device;
extern t800::DeviceContext*     D3D11DeviceContext;

void D3DXQuad::Create() {
  ID3D11Device* device = reinterpret_cast<ID3D11Device*>(*D3D11Device->GetAPIDevice());
  ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());
	SigBase = Signature::HAS_TEXCOORDS0;
	unsigned int Dest;
	char *vsSourceP = file2string("Shaders/VS_Quad.hlsl");
	char *fsSourceP = file2string("Shaders/FS_Quad.hlsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	free(vsSourceP);
	free(fsSourceP);

	int shaderID = g_pBaseDriver->CreateShader(vstr, fstr, SigBase);

	Dest = SigBase | Signature::DEFERRED_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_1_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_2_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_3_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::SHADOW_COMP_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::VERTICAL_BLUR_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::HORIZONTAL_BLUR_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::ONE_PASS_BLUR;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::BRIGHT_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::HDR_COMP_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);
	
	vertices[0] = { -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[1] = { -1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[2] = {  1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
	vertices[3] = {  1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f };

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;

	D3DXShader* s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderIdx(shaderID));

	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.ByteWidth = sizeof(CBuffer);
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = device->CreateBuffer(&bdesc, 0, pd3dConstantBuffer.GetAddressOf());
	if (hr != S_OK) {
		printf("Error Creating Constant Buffer\n");
		return;
	}

	bdesc = { 0 };
	bdesc.ByteWidth = sizeof(Vert) * 4;
	bdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };

	hr = device->CreateBuffer(&bdesc, &subData, &VB);
	if (hr != S_OK) {
		printf("Error Creating Vertex Buffer\n");
		return;
	}

	bdesc = { 0 };
	bdesc.ByteWidth = 6 * sizeof(USHORT);
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	subData = { indices, 0, 0 };

	hr = device->CreateBuffer(&bdesc, &subData, &IB);
	if (hr != S_OK) {
		printf("Error Creating Index Buffer\n");
		return;
	}

	D3D11_SAMPLER_DESC sdesc;
	sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.MinLOD = 0;
	sdesc.MaxLOD = D3D11_FLOAT32_MAX;
	sdesc.MipLODBias = 0.0f;
	sdesc.MaxAnisotropy = 1;
	sdesc.BorderColor[0] = sdesc.BorderColor[1] = sdesc.BorderColor[2] = sdesc.BorderColor[3] = 0;
	device->CreateSamplerState(&sdesc, pSampler.GetAddressOf());

	XMatIdentity(transform);
}

void D3DXQuad::Transform(float *t) {
	transform = t;
}

void D3DXQuad::Draw(float *t, float *vp) {

	if (t)
		transform = t;
  ID3D11Device* device = reinterpret_cast<ID3D11Device*>(*D3D11Device->GetAPIDevice());
  ID3D11DeviceContext* deviceContext = reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext());
	unsigned int sig = SigBase;
	sig |= gSig;
	D3DXShader * s = dynamic_cast<D3DXShader*>(g_pBaseDriver->GetShaderSig(sig));
	UINT offset = 0;
	UINT stride = sizeof(Vert);

	for (int i = 0; i < 8; i++) {
		d3dxTextures[i] = dynamic_cast<D3DXTexture*>(Textures[i]);
	}

	d3dxEnvMap = dynamic_cast<D3DXTexture*>(EnvMap);

	Camera *pActualCamera = pScProp->pCameras[0];
	XMATRIX44 VP = pActualCamera->VP;
	XMATRIX44 WV = pActualCamera->View;
	VP.Inverse(&CnstBuffer.WVPInverse);
	CnstBuffer.WVP = transform;
	CnstBuffer.World = transform;
	CnstBuffer.WorldView = WV;
	CnstBuffer.CameraPos = pActualCamera->Eye;

	if (pScProp->pLightCameras.size() > 0) {
		CnstBuffer.WVPLight = pScProp->pLightCameras[0]->VP;
		CnstBuffer.LightCameraPos = pScProp->pLightCameras[0]->Eye;
		CnstBuffer.LightCameraInfo = XVECTOR3(pScProp->pLightCameras[0]->NPlane, pScProp->pLightCameras[0]->FPlane, pScProp->pLightCameras[0]->Fov, 1.0f);
	}

	if (sig&Signature::DEFERRED_PASS) {
		unsigned int numLights = pScProp->ActiveLights;
		if (numLights >= pScProp->Lights.size())
			numLights = pScProp->Lights.size();

		CnstBuffer.CameraInfo = XVECTOR3(pActualCamera->NPlane, pActualCamera->FPlane, pActualCamera->Fov, float(numLights));

		for (unsigned int i = 0; i < numLights; i++) {
			CnstBuffer.LightPositions[i] = pScProp->Lights[i].Position;
			CnstBuffer.LightColors[i] = pScProp->Lights[i].Color;
		}
	}else if (sig&Signature::ONE_PASS_BLUR) {
		CnstBuffer.LightPositions[0].x = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].x;
		CnstBuffer.LightPositions[0].y = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].y;
		CnstBuffer.LightPositions[0].z = (float)Textures[0]->x;
		CnstBuffer.LightPositions[0].w = (float)Textures[0]->y;
		for (unsigned int i = 1; i < pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel.size(); i++) {
			CnstBuffer.LightPositions[i] = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[i];
		}	
	}else if (sig&Signature::VERTICAL_BLUR_PASS || sig&Signature::HORIZONTAL_BLUR_PASS) {
		CnstBuffer.LightPositions[0].x = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].x;
		CnstBuffer.LightPositions[0].y = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].y;
		CnstBuffer.LightPositions[0].z = (float)Textures[0]->x;
		CnstBuffer.LightPositions[0].w = (float)Textures[0]->y;
		for (unsigned int i = 1; i < pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel.size(); i++) {
			CnstBuffer.LightPositions[i].x = roundTo(pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[i].x , 6.0f);
		}
	}else if (sig&Signature::HDR_COMP_PASS || sig&Signature::BRIGHT_PASS || sig&Signature::FSQUAD_3_TEX){
		D3D11_TEXTURE2D_DESC pDesc;
		d3dxTextures[0]->Tex->GetDesc(&pDesc);
		CnstBuffer.CameraPos.w = (float)pDesc.MipLevels;
		CnstBuffer.LightPositions[0].x = pScProp->BloomFactor;
		CnstBuffer.LightPositions[0].y = pScProp->Exposure;
	}

	deviceContext->VSSetShader(s->pVS.Get(), 0, 0);
  deviceContext->PSSetShader(s->pFS.Get(), 0, 0);
  deviceContext->IASetInputLayout(s->Layout.Get());

  deviceContext->UpdateSubresource(pd3dConstantBuffer.Get(), 0, 0, &CnstBuffer, 0, 0);
  deviceContext->VSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());
  deviceContext->PSSetConstantBuffers(0, 1, pd3dConstantBuffer.GetAddressOf());

  deviceContext->IASetIndexBuffer(IB.Get(), DXGI_FORMAT_R16_UINT, 0);
  deviceContext->IASetVertexBuffers(0, 1, VB.GetAddressOf(), &stride, &offset);

	if (sig&Signature::DEFERRED_PASS) {
		deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(1, 1, d3dxTextures[1]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(2, 1, d3dxTextures[2]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(3, 1, d3dxTextures[3]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(4, 1, d3dxTextures[4]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(5, 1, d3dxTextures[5]->pSRVTex.GetAddressOf());
		deviceContext->PSSetShaderResources(6, 1, d3dxEnvMap->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::FSQUAD_1_TEX || sig&Signature::BRIGHT_PASS) {
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::FSQUAD_2_TEX) {
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
    deviceContext->PSSetShaderResources(1, 1, d3dxTextures[1]->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::FSQUAD_3_TEX || sig&Signature::HDR_COMP_PASS) {			
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
    deviceContext->PSSetShaderResources(1, 1, d3dxTextures[1]->pSRVTex.GetAddressOf());
    deviceContext->PSSetShaderResources(2, 1, d3dxTextures[2]->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::SHADOW_COMP_PASS) {
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
    deviceContext->PSSetShaderResources(1, 1, d3dxTextures[1]->pSRVTex.GetAddressOf());
	}
	else if (sig&Signature::VERTICAL_BLUR_PASS || sig&Signature::HORIZONTAL_BLUR_PASS) {
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
	}
	else {
    deviceContext->PSSetShaderResources(0, 1, d3dxTextures[0]->pSRVTex.GetAddressOf());
	}

	

  deviceContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  deviceContext->DrawIndexed(6, 0, 0);
}

void D3DXQuad::Destroy(){
}
