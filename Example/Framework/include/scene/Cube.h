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

#ifndef UAD_CUBE_GL_H
#define UAD_CUBE_GL_H

#include <Config.h>

#include <utils\Utils.h>

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <video/TextureGL.h>
#elif defined(USING_D3D11)
#include <video\TextureD3D.h>
#include <D3Dcompiler.h>
#endif

#include <utils\xMaths.h>
#include <scene\PrimitiveBase.h>


struct CVertex {

	float x, y, z,w;
	float nx,ny,nz,nw;
	float s,t;
};



class Cube : public PrimitiveBase {
public:
	Cube() 
#ifdef USING_OPENGL_ES
	: shaderID(0)
#endif
	 {}

#ifdef USING_D3D11
	struct CBuffer {
		XMATRIX44 WVP;
		XMATRIX44 World;
	};
#endif

	void Create();
	void Create(char *) {}
	void Transform(float *t);
	void Draw(float *t,float *vp);
	void Destroy();

#ifdef USING_OPENGL_ES
	GLuint	shaderID;
	GLint	vertexAttribLoc;
	GLint	normalAttribLoc;
	GLint	uvAttribLoc;
	GLint	diffuseLoc;
	

	GLint  matWorldViewProjUniformLoc;
	GLint  matWorldUniformLoc;

	GLuint			VB;
	GLuint			IB;
#elif defined(USING_D3D11)
	ComPtr<ID3D11Buffer>		IB;
	ComPtr<ID3D11Buffer>		VB;
	ComPtr<ID3D11VertexShader>  pVS;
	ComPtr<ID3D11PixelShader>   pFS;
	ComPtr<ID3DBlob>            VS_blob;
	ComPtr<ID3DBlob>            FS_blob;
	ComPtr<ID3D11InputLayout>   Layout;
	ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

	Cube::CBuffer	CnstBuffer;
#endif

	int		TexId;
	CVertex			vertices[24];
	unsigned short	indices[36];
	XMATRIX44	transform;
	Texture		*tex;
};


#endif