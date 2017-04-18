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

#ifndef T800_QUAD_D3DX_H
#define T800_QUAD_D3DX_H

#include <Config.h>

#include <video\BaseDriver.h>
#include <video\D3DXTexture.h>
#include <utils\xMaths.h>
#include <scene\PrimitiveBase.h>

class D3DXQuad : public PrimitiveBase {
public:
	struct Vert {
		float x, y, z, w;
		float u, v;
	};

	struct CBuffer {
		XMATRIX44 WVP;
		XMATRIX44 World;
		XMATRIX44 WorldView;
		XMATRIX44 WVPInverse;
		XVECTOR3  LightPositions[128];
		XVECTOR3  LightColors[128];
		XVECTOR3  CameraPos;
		XVECTOR3  CameraInfo;		
	};

	D3DXQuad() {}
	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	unsigned int	SigBase;

	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;
	ComPtr<ID3D11SamplerState>  pSampler;

	CBuffer			CnstBuffer;
	Vert			vertices[4];
	unsigned short	indices[6];

	XMATRIX44		transform;
	

	D3DXTexture*	d3dxTextures[8];

};

#endif