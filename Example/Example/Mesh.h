#ifndef UAD_MESH_GL_H
#define UAD_MESH_GL_H

#include "Config.h"

#ifdef USING_OPENGL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureGL.h"
#elif defined(USING_D3D11)
#include "TextureD3D.h"
#include <D3Dcompiler.h>
#endif

#include <xMaths.h>
#include <XDataBase.h>

#include "PrimitiveBase.h"
#include "Utils.h"


#include <vector>

using namespace xF;

class Mesh : public PrimitiveBase {
public:
	Mesh()  {}

#ifdef USING_D3D11
	struct CBuffer {
		XMATRIX44 WVP;
		XMATRIX44 World;
	};
#endif

	struct SubSetInfo {
#ifdef USING_OPENGL_ES
		unsigned long		VertexAttrib;
		unsigned int		Id;
		unsigned int		IdTex;
		unsigned int		IdTexUniformLoc;
#elif defined(USING_D3D11)
		ComPtr<ID3D11Buffer>		IB;
		Texture*					DiffTex;
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
		unsigned int ShaderProg;
		unsigned int			 Id;
		unsigned int			 IdIBO;
		unsigned int			 matWorldViewProjUniformLoc;
		unsigned int			 matWorldUniformLoc;

		int			 vertexAttribLoc;
		int			 normalAttribLoc;
		int			 tangentAttribLoc;
		int			 binormalAttribLoc;
		int			 uvAttribLoc;
		int			 uvSecAttribLoc;
			
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
		std::vector<SubSetInfo>	SubSets;
	};

	

	void Create() {}
	void Create(char *);
	void Transform(float *t);
	void Draw(float *t, float *vp);
	void Destroy();

	void GatherInfo();

	XMATRIX44	transform;
	XDataBase	xFile;
	std::vector<Texture*> Textures;
	std::vector<MeshInfo> Info;
;
};


#endif