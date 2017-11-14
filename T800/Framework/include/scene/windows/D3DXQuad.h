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
#include <utils\xMaths.h>
#include <scene\PrimitiveBase.h>


#if defined(USING_OPENGL)
#include <video\GLTexture.h>
#else
#include <video\windows/D3DXTexture.h>
#endif

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
    XMATRIX44 WVPLight;
    XVECTOR3  LightPositions[128];
    XVECTOR3  LightColors[128];
    XVECTOR3  CameraPos;
    XVECTOR3  CameraInfo;
    XVECTOR3  LightCameraPos;
    XVECTOR3  LightCameraInfo;
  };

  D3DXQuad() {
    for (int i = 0; i < 8; i++) {
      d3dxTextures[i] = 0;
    }
    d3dxEnvMap = 0;
  }
  void Create();
  void Create(char *) {}
  void Transform(float *t);
  void Draw(float *t, float *vp);
  void Destroy();

  unsigned int	SigBase;

  t800::IndexBuffer*		IB;
  t800::VertexBuffer*		VB;
  t800::ConstantBuffer*        pd3dConstantBuffer;
  //ID3D11SamplerState*  pSampler;

  CBuffer			CnstBuffer;
  Vert			vertices[4];
  unsigned short	indices[6];

  XMATRIX44		transform;


  Texture*	d3dxTextures[8];
  Texture*	d3dxEnvMap;

};

#endif