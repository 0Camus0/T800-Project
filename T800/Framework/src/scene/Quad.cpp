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

#include <scene/Quad.h>
#include <utils/Utils.h>

#include <video/GLShader.h>
#include <video/GLDriver.h>
#if defined(OS_WINDOWS)
#include <video/windows/D3DXShader.h>
#include <video/windows/D3DXDriver.h>
#endif
namespace t800 {
  extern Device*            D3D11Device;
  extern DeviceContext*     D3D11DeviceContext;

  void D3DXQuad::Create() {
    SigBase = Signature::HAS_TEXCOORDS0;
    unsigned int Dest;

    char *vsSourceP;
    char *fsSourceP;
    if (g_pBaseDriver->m_currentAPI == GRAPHICS_API::OPENGL) {
    vsSourceP = file2string("Shaders/VS_Quad.glsl");
    fsSourceP = file2string("Shaders/FS_Quad.glsl");
  }
    else {
    vsSourceP = file2string("Shaders/VS_Quad.hlsl");
    fsSourceP = file2string("Shaders/FS_Quad.hlsl");
    }


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
    vertices[2] = { 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f };
    vertices[3] = { 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f };

    indices[0] = 2;
    indices[1] = 1;
    indices[2] = 0;
    indices[3] = 3;
    indices[4] = 2;
    indices[5] = 0;

    ShaderBase* s = g_pBaseDriver->GetShaderIdx(shaderID);


    t800::BufferDesc bdesc;
    bdesc.byteWidth = sizeof(CBuffer);
    bdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    pd3dConstantBuffer = (t800::ConstantBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::CONSTANT, bdesc);

    bdesc.byteWidth = sizeof(Vert) * 4;
    bdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    VB = (t800::VertexBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::VERTEX, bdesc, vertices);


    bdesc.byteWidth = 6 * sizeof(unsigned short);
    bdesc.usage = T8_BUFFER_USAGE::DEFAULT;
    IB = (t800::IndexBuffer*)D3D11Device->CreateBuffer(T8_BUFFER_TYPE::INDEX, bdesc, indices);

    /*D3D11_SAMPLER_DESC sdesc;
    sdesc.Filter = D3D11_FILTER_ANISOTROPIC;
    sdesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sdesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sdesc.MinLOD = 0;
    sdesc.MaxLOD = D3D11_FLOAT32_MAX;
    sdesc.MipLODBias = 0.0f;
    sdesc.MaxAnisotropy = 1;
    sdesc.BorderColor[0] = sdesc.BorderColor[1] = sdesc.BorderColor[2] = sdesc.BorderColor[3] = 0;
    reinterpret_cast<ID3D11Device*>(*D3D11Device->GetAPIDevice())->CreateSamplerState(&sdesc, &pSampler);*/

    XMatIdentity(transform);
  }

  void D3DXQuad::Transform(float *t) {
    transform = t;
  }

  void D3DXQuad::Draw(float *t, float *vp) {

    if (t)
      transform = t;
    unsigned int sig = SigBase;
    sig |= gSig;
    ShaderBase * s = g_pBaseDriver->GetShaderSig(sig);
    unsigned int offset = 0;
    unsigned int stride = sizeof(Vert);

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
    }
    else if (sig&Signature::ONE_PASS_BLUR) {
      CnstBuffer.LightPositions[0].x = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].x;
      CnstBuffer.LightPositions[0].y = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].y;
      CnstBuffer.LightPositions[0].z = (float)Textures[0]->x;
      CnstBuffer.LightPositions[0].w = (float)Textures[0]->y;
      for (unsigned int i = 1; i < pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel.size(); i++) {
        CnstBuffer.LightPositions[i] = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[i];
      }
    }
    else if (sig&Signature::VERTICAL_BLUR_PASS || sig&Signature::HORIZONTAL_BLUR_PASS) {
      CnstBuffer.LightPositions[0].x = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].x;
      CnstBuffer.LightPositions[0].y = pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[0].y;
      CnstBuffer.LightPositions[0].z = (float)Textures[0]->x;
      CnstBuffer.LightPositions[0].w = (float)Textures[0]->y;
      for (unsigned int i = 1; i < pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel.size(); i++) {
        CnstBuffer.LightPositions[i].x = roundTo(pScProp->pGaussKernels[pScProp->ActiveGaussKernel]->vGaussKernel[i].x, 6.0f);
      }
    }
    else if (sig&Signature::HDR_COMP_PASS || sig&Signature::BRIGHT_PASS || sig&Signature::FSQUAD_3_TEX) {
      //D3D11_TEXTURE2D_DESC pDesc;
      //reinterpret_cast<D3DXTexture*>(d3dxTextures[0])->Tex->GetDesc(&pDesc);
      CnstBuffer.CameraPos.w = 10.0f;//(float)pDesc.MipLevels;
      CnstBuffer.LightPositions[0].x = pScProp->BloomFactor;
      CnstBuffer.LightPositions[0].y = pScProp->Exposure;
    }
    VB->Set(*D3D11DeviceContext, stride, offset);
    IB->Set(*D3D11DeviceContext, 0, T8_IB_FORMAR::R16);
    s->Set(*D3D11DeviceContext);

    pd3dConstantBuffer->UpdateFromBuffer(*D3D11DeviceContext, &CnstBuffer);
    pd3dConstantBuffer->Set(*D3D11DeviceContext);

    if (sig&Signature::DEFERRED_PASS) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
      Textures[1]->Set(*D3D11DeviceContext, 1, "tex1");
      Textures[2]->Set(*D3D11DeviceContext, 2, "tex2");
      Textures[3]->Set(*D3D11DeviceContext, 3, "tex3");
      Textures[4]->Set(*D3D11DeviceContext, 4, "tex4");
      Textures[5]->Set(*D3D11DeviceContext, 5, "tex5");
      EnvMap->Set(*D3D11DeviceContext, 6, "texEnv");
    }
    else if (sig&Signature::FSQUAD_1_TEX || sig&Signature::BRIGHT_PASS) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
    }
    else if (sig&Signature::FSQUAD_2_TEX) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
      Textures[1]->Set(*D3D11DeviceContext, 1, "tex1");
    }
    else if (sig&Signature::FSQUAD_3_TEX || sig&Signature::HDR_COMP_PASS) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
      Textures[1]->Set(*D3D11DeviceContext, 1, "tex1"); 
      Textures[2]->Set(*D3D11DeviceContext, 2, "tex2"); 
    }
    else if (sig&Signature::SHADOW_COMP_PASS) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
      Textures[1]->Set(*D3D11DeviceContext, 1, "tex1");
    }
    else if (sig&Signature::VERTICAL_BLUR_PASS || sig&Signature::HORIZONTAL_BLUR_PASS) {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
    }
    else {
      Textures[0]->Set(*D3D11DeviceContext, 0, "tex0");
    }
    //reinterpret_cast<ID3D11DeviceContext*>(*D3D11DeviceContext->GetAPIContext())->PSSetSamplers(0, 1, &pSampler);

    D3D11DeviceContext->SetPrimitiveTopology(T8_TOPOLOGY::TRIANLE_LIST);
    D3D11DeviceContext->DrawIndexed(6, 0, 0);
  }

  void D3DXQuad::Destroy() {
    IB->release();
    VB->release();
    pd3dConstantBuffer->release();
  }
}


