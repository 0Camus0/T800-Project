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

#ifndef T800_MESH_GL_H
#define T800_MESH_GL_H

#include <Config.h>

#ifdef USING_OPENGL_ES20
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <video/TextureGL.h>
#elif defined(USING_OPENGL_ES30)
#include <GLES3/gl31.h>
#include <video/TextureGL.h>
#elif defined(USING_OPENGL)
#include <GL/glew.h>
#include <video/TextureGL.h>
#elif defined(USING_D3D11)
#include <video\TextureD3D.h>
#include <D3Dcompiler.h>
#endif

#include <video\BaseDriver.h>

#include <utils\Utils.h>

#include <utils\xMaths.h>
#include <utils\XDataBase.h>
#include <scene\PrimitiveBase.h>



#include <vector>

using namespace xF;

class Mesh : public PrimitiveBase {
public:
	Mesh()  {}

#ifdef USING_D3D11
	struct CBuffer {
		XMATRIX44 WVP;
		XMATRIX44 World;
		XVECTOR3  Light0Pos;
		XVECTOR3  Light0Col;
		XVECTOR3  CameraPos;
		XVECTOR3  Ambient;
	};
#endif

	struct SubSetInfo {
		
		unsigned int		Sig;
#ifdef USING_GL_COMMON
		unsigned long		VertexAttrib;
		unsigned int		Id;
		unsigned int		IdDiffuseTex;
		unsigned int		IdSpecularTex;
		unsigned int		IdGlossTex;
		unsigned int		IdNormalTex;
		unsigned int		IdReflectTex;
		unsigned int		IdTexUniformLoc;
#elif defined(USING_D3D11)
		ComPtr<ID3D11Buffer>		IB;
		Texture*					DiffuseTex;
		Texture*					SpecularTex;
		Texture*					GlossfTex;
		Texture*					NormalTex;
		Texture*					ReflectTex;
#endif
		unsigned int		VertexStart;
		unsigned int		NumVertex;
		unsigned int		TriStart;
		unsigned int		NumTris;
		unsigned int		VertexSize;
		bool				bAlignedVertex;
	};


	struct MeshInfo {
		unsigned int			 VertexSize;
		unsigned int			 NumVertex;

#ifdef USING_GL_COMMON
		unsigned int			 Id;
		unsigned int			 IdIBO;			
#elif defined(USING_D3D11)
		ComPtr<ID3D11Buffer>		IB;
		ComPtr<ID3D11Buffer>		VB;
		ComPtr<ID3D11Buffer>        pd3dConstantBuffer;
		Mesh::CBuffer				CnstBuffer;
#endif
		std::vector<SubSetInfo>	SubSets;
	};

	

	void Create() {}
	void Create(char *);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	void GatherInfo();
	Texture* LoadTex(std::string p, xF::xMaterial *mat);

	XMATRIX44	transform;
	XDataBase	xFile;
	std::vector<Texture*> Textures;
	std::vector<MeshInfo> Info;


};


#endif