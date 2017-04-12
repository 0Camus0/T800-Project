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

#include <video\D3DXShader.h>
#include <scene\D3DXQuad.h>
#include <utils\Utils.h>

void D3DXQuad::Create() {
	SigBase = Signature::HAS_TEXCOORDS0;
	unsigned int Dest;
	char *vsSourceP = file2string("Shaders/VS_Quad.glsl");
	char *fsSourceP = file2string("Shaders/FS_Quad.glsl");

	std::string vstr = std::string(vsSourceP);
	std::string fstr = std::string(fsSourceP);

	free(vsSourceP);
	free(fsSourceP);

	g_pBaseDriver->CreateShader(vstr, fstr, SigBase);

	Dest = SigBase | Signature::DEFERRED_PASS;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_1_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_2_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	Dest = SigBase | Signature::FSQUAD_3_TEX;
	g_pBaseDriver->CreateShader(vstr, fstr, Dest);

	vertices[0] = { -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f };
	vertices[1] = { -1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 0.0f };
	vertices[2] = { 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f };
	vertices[3] = { 1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f };

	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 0;
}

void D3DXQuad::Transform(float *t) {

}

void D3DXQuad::Draw(float *t, float *vp) {

}

void D3DXQuad::Destroy(){
}