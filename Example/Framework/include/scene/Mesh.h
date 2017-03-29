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

#ifndef UAD_MESH_GL_H
#define UAD_MESH_GL_H

#include <Config.h>

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <video/TextureGL.h>
#elif defined(USING_D3D11)
#include <video\TextureD3D.h>
#include <D3Dcompiler.h>
#endif

#include <utils\Utils.h>

#include <utils\xMaths.h>
#include <utils\XDataBase.h>
#include <scene\PrimitiveBase.h>



#include <vector>

using namespace xF;

class Mesh : public PrimitiveBase {
public:
	Mesh()  {}

	enum Signature {
		DIFFUSE_MAP = 1,
		SPECULAR_MAP = 2,
		GLOSS_MAP = 4,
		NORMAL_MAP = 8,
		REFLECT_MAP = 16,

		HAS_NORMALS		= 32,
		HAS_TANGENTS	= 64,
		HAS_BINORMALS	= 128,
		HAS_TEXCOORDS0	= 256,
		HAS_TEXCOORDS1  = 512,

		NO_LIGHT_AT_ALL = 1024
	};

	struct Shader {
		Shader() : Sig(0), MeshIndex(0){
#ifdef USING_D3D11
#elif defined(USING_OPENGL_ES)
			matWorldViewProjUniformLoc = -1;
			matWorldUniformLoc = -1;

			vertexAttribLoc = -1;
			normalAttribLoc = -1;
			tangentAttribLoc = -1;
			binormalAttribLoc = -1;
			uvAttribLoc = -1;
			uvSecAttribLoc = -1;

			Light0Pos_Loc = -1;
			Light0Color_Loc = -1;
			CameraPos_Loc = -1;
			Ambient_loc = -1;
			DiffuseTex_loc = -1;
			SpecularTex_loc = -1;
			GlossTex_loc = -1;
			NormalTex_loc = -1;
			ReflectTex_loc = -1;
#endif
		}
		int							Sig;
		int							MeshIndex;
#ifdef USING_D3D11
		ComPtr<ID3D11VertexShader>  pVS;
		ComPtr<ID3D11PixelShader>   pFS;
		ComPtr<ID3DBlob>            VS_blob;
		ComPtr<ID3DBlob>            FS_blob;
#elif defined(USING_OPENGL_ES)
		unsigned int ShaderProg;

		int			 matWorldViewProjUniformLoc;
		int			 matWorldUniformLoc;

		int			 vertexAttribLoc;
		int			 normalAttribLoc;
		int			 tangentAttribLoc;
		int			 binormalAttribLoc;
		int			 uvAttribLoc;
		int			 uvSecAttribLoc;

		int			Light0Pos_Loc;
		int			Light0Color_Loc;

		int			CameraPos_Loc;

		int			Ambient_loc;

		int			DiffuseTex_loc;
		int			SpecularTex_loc;
		int			GlossTex_loc;
		int			NormalTex_loc;
		int			ReflectTex_loc;
#endif
	};
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
#ifdef USING_OPENGL_ES
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

#ifdef USING_OPENGL_ES
		unsigned int			 Id;
		unsigned int			 IdIBO;

		unsigned int ShaderProg;
		
		int			 matWorldViewProjUniformLoc;
		int			 matWorldUniformLoc;

		int			 vertexAttribLoc;
		int			 normalAttribLoc;
		int			 tangentAttribLoc;
		int			 binormalAttribLoc;
		int			 uvAttribLoc;
		int			 uvSecAttribLoc;

		int			Light0Pos_Loc;
		int			Light0Color_Loc;

		int			CameraPos_Loc;

		int			Ambient_loc;
			
#elif defined(USING_D3D11)
		ComPtr<ID3D11Buffer>		IB;
		ComPtr<ID3D11Buffer>		VB;
		ComPtr<ID3D11VertexShader>  pVS;
		ComPtr<ID3D11PixelShader>   pFS;
		ComPtr<ID3DBlob>            VS_blob;
		ComPtr<ID3DBlob>            FS_blob;
		ComPtr<ID3D11InputLayout>   Layout;
		ComPtr<ID3D11Buffer>        pd3dConstantBuffer;

		std::vector<D3D11_INPUT_ELEMENT_DESC>	VertexDecl;

		Mesh::CBuffer				CnstBuffer;
#endif
		std::vector<Shader>		Shaders;
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